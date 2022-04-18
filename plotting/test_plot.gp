set terminal wxt size 1300,600
set multiplot layout 3, 1

# You can literally replace everything after plot with just u in stead of using etc
# Don't do that for readability here.
plot filename using 1:2 title 'left' with lines
plot filename using 1:3 title 'right' with lines
plot filename using 1:4 title 'rms' with lines

unset multiplot