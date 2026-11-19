#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define ARG_FLAGS_VARIANCE 1
#define NO_ARG_FLAGS_VARIANCE 0

static struct option _LongOptionList[] = {
    { "return-sigma", 0, NULL, 's' },
    { "number-n", 1, NULL, 'n' },
    { "file", 1, NULL, 'f' },
    { NULL, 0, NULL, 0 },
};

unsigned int ArgEnableVariance = NO_ARG_FLAGS_VARIANCE;
uint64_t ArgN = 0;
char *FileName;

void process_arguments (int argc, char **argv)
{
    int _Ret;

    while (1) {
        _Ret = getopt_long (argc, argv, "snf:", _LongOptionList, NULL);

        if (_Ret < 0) {
            break;
        }

        switch (_Ret) {
            case 's':
                ArgEnableVariance = ARG_FLAGS_VARIANCE;
                break;

            case 'n':
                ArgN = atoi(optarg);

                if (ArgN <= 0) {
                    fprintf(stderr, "N is not positive!\n");
                }

                break;

            case 'f':
                FileName = strdup(optarg);

                if (FileName == "") {
                    fprintf(stderr, "File name is not valid!\n");
                }

                break;

            case '?':
                break;
        }
    }
}

bool IsBitSet(uint8_t num, int bit)
{
    return 1 == ((num >> bit) & 1);
}

int32_t BufferToInt(unsigned char buffer[4])
{
    int32_t concatInt;

    if (IsBitSet(buffer[2], 0)) //handles signum of integers
    {
        concatInt = ((int32_t)0xFF << 24) | ((int32_t)0xFF << 16) | ((int32_t)buffer[1] << 8) | (buffer[0]);
    } else {
        concatInt = ((int32_t)0x00 << 24) | ((int32_t)0x00 << 16) | ((int32_t)buffer[1] << 8) | (buffer[0]);
    }

    return concatInt;
}

int main (int argc, char **argv)
{
    FILE *fp;
    FILE *fp2;
    FILE *fw;
    unsigned char buffer[4];
    bool next = true;

    int32_t mean = 0;
    int32_t sum = 0;
    int32_t toReturn = 0;
    int32_t concatInt;
    double sumSigma = 0;

    fp = fopen(FileName, "rb");
    fp2 = fopen(FileName, "rb");
    fw = fopen("plotData.dat", "w");

    process_arguments(argc, argv);

    while (next)
    {

        for (int i = 0; i < ArgN; i++)
        {
            if (fread(buffer, 4, 1, fp) != 1)
            {
                next = false;
                break;
            }

            concatInt = BufferToInt(buffer);

            sum += concatInt;
        }

        mean = (sum > 0) ? sum/ArgN : -((-sum)/ArgN);

        if (ArgEnableVariance == ARG_FLAGS_VARIANCE)
        {

            for (int i = 0; i < ArgN; i++)
            {
                if (fread(buffer, 4, 1, fp2) != 1)
                {
                    next = false;
                    break;
                }

                concatInt = BufferToInt(buffer);

                sumSigma += ((concatInt - mean)*(concatInt - mean))/ArgN;
            }

            toReturn = sumSigma;
        }
        else
        {
            toReturn = mean;
        }

        fprintf(fw, "%d\n", toReturn);
    }

    return 0;
}
