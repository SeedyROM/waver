#!/bin/bash

OUTPUT_FILE=/tmp/wav_data.dat

make clean
make
./build/a.out > $OUTPUT_FILE
gnuplot -p -e "filename='$OUTPUT_FILE'" ./plotting/test_plot.gp