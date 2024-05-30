#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>

static struct option _LongOptionList[] = {
    { "return-sigma", 0, NULL, 's' },
    { "number-n",  1, NULL, 'n' },
    { NULL,       0, NULL, 0   },
};

typedef struct Parameters {
    bool flagPresent;
    int n;
} parameters;

struct Parameters process_arguments (int argc, char **argv)
{
    parameters p;
    p.flagPresent = false;

    int _Ret;

    while (1) {
        _Ret = getopt_long (argc, argv, "-fv:", _LongOptionList, NULL);

        if (_Ret < 0) {
            break;
        }

        switch (_Ret) {
            case 's':
                p.flagPresent = true;
                break;

            case 'n':
                p.n = atoi(optarg);

                break;

            case '?':
                break;

            case 1:
                break;
        }
    }

    return p;
}

bool IsBitSet(uint8_t num, int bit)
{
    return 1 == ( (num >> bit) & 1);
}

unsigned char Reverse(unsigned char b) //mirrors bytes
{
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

int32_t BufferToInt(unsigned char buffer[4])
{
    int32_t concatInt = (Reverse(buffer[3]) << 24) | (Reverse(buffer[2]) << 16) | (Reverse(buffer[1]) << 8) | Reverse(buffer[0]);

    if (IsBitSet(buffer[2], 0)) //handles signum of integers
    {
        concatInt = concatInt & 0x0000FFFF;
    }
    else
    {
        concatInt = concatInt | 0xFFFF0000;
    }

    return concatInt;
}

int main (int argc, char **argv)
{
    FILE *fp;
    FILE *fw;
    unsigned char buffer[4];
    int n;
    bool next = true;

    parameters p;

    fp = fopen("11GNU-data1.dat", "rb");
    fw = fopen("plotData.dat", "w");
    p = process_arguments(argc, argv);
    n = p.n;

    while (next)
    {
        int32_t sum = 0;
        int toReturn = 0;

        int values[n];

        for (int i = 0; i < n; i++)
        {
            if (fread(buffer, 4, 1, fp) != 1)
            {
                next = false;
                break;
            }

            int32_t concatInt = BufferToInt(buffer);

            if (p.flagPresent)
            {
                values[i] = concatInt;
            }

            sum += concatInt;
        }

        int mean = sum / n;

        if (p.flagPresent)
        {
            long long sum2 = 0;

            for (int i = 0; i < n; i++)
            {
                sum2 += ((values[i] - mean)*(values[i] - mean))/n;
            }

            toReturn = sum2;
        }
        else
        {
            toReturn = mean;
        }

        fprintf(fw, "%d\n", toReturn);
    }

    return 0;
}
