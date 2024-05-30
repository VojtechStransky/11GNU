while getopts ":f:n:s:" opt; do
  case ${opt} in
    f ) file=$OPTARG;;
    n ) number=$OPTARG;;
    s ) sigma=$OPTARG;;
    \? ) echo "Usage: cmd [-f] [-n] [-s]";;
  esac
done

preGain=$(sed -nr '/#\s+Pre-Amplifier\sGain/{s/.*Pre-Amplifier\sGain\s?+:\s?+(([0-9]+.?[0-9]+)).*/\1/;p;}' $file)
gain=$(sed -nr '/#\s+Amplifier\sGain/{s/.*Amplifier\sGain\s?+:\s?+(([0-9]+.?[0-9]+)).*/\1/;p;}' $file)
frequency=$(sed -nr '/#\s+Sampling\sFreq.:/{s/.*Sampling\sFreq.\s?+:\s?+(([0-9]+.?[0-9]+)).*/\1/;p;}' $file)

if [ "$sigma" = true ] ; then
    ./a.out --number-n=$number --return-sigma
    yLabel="Variance[V^{2}]"
else
    ./a.out --number-n=$number
    yLabel="Mean[V]"
fi

gnuplot -e "gainTotal=$gain" -e "yLabelText='$yLabel'" plot.gpl

