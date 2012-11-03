#
set title "Wall clock (seconds) vs. block size (bytes)"
set xlabel "Block size (bytes)"
set ylabel "Wallclock time (seconds)"

set logscale x
#set yrange [1:50]
# set ytics (1,2,5,10,20,50)
# set format y "%2.0t{/Symbol \327}10^{%L}"
# set lmargin 8
set terminal postscript eps enhanced
set output 'plot.eps'

plot "times.dat" with lines
#pause -1 "Hit any key to continue"
