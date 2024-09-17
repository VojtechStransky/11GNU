#!/bin/bash

while getopts ":f:d:n:s:" opt; do
  case ${opt} in
    f ) fileDat=$OPTARG;;
    d ) file=$OPTARG;;
    n ) number=$OPTARG;;
    s ) sigma=$OPTARG;;
    \? ) echo "Usage: cmd [-f] [-n] [-s]";;
  esac
done

preGain=$(sed -nr '/^#\s*Pre-Amplifier\sGain/{s/.*Pre-Amplifier\sGain\s*:\s*(([0-9]+.?[0-9]+)).*/\1/;p;}' $file)
gain=$(sed -nr '/^#\s*Amplifier\sGain/{s/.*Amplifier\sGain\s*:\s*(([0-9]+.?[0-9]+)).*/\1/;p;}' $file)
frequency=$(sed -nr '/^#\s*Sampling\sFreq.:/{s/.*Sampling\sFreq.\s*:\s*(([0-9]+.?[0-9]+)).*/\1/;p;}' $file)

if [ "$sigma" = true ] ; then
    gnucode --number-n=$number --return-sigma --file=$fileDat
    yLabel="Variance[V^{2}]"
else
    gnucode --number-n=$number --file=$fileDat
    yLabel="Mean[V]"
fi

gnuplot -e "gainTotal=$gain" -e "yLabelText='$yLabel'" -e "frequency=$frequency" plot.gpl

