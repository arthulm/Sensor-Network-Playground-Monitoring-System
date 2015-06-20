set terminal postscript eps enhanced color

set key out spacing 1.1
set key box
set key below width 3

set ytics nomirror # Switch mirroring of primary system OFF 
set y2tics	# Switch secondary system ON

set xlabel "time [system ticks]"
#set ylabel "time spent [system ticks]"
set ylabel "sample"

set style line 1 lt 1 lc rgb "blue" lw 1 pt 1
set style line 2 lt 1 lc rgb "green" lw 1 pt 2
set style line 3 lt 1 lc rgb "red" lw 1 pt 3
#set style line 4 lt 1 lc rgb "yellow" lw 1 pt 4
#set style line 5 lt 1 lc rgb "black" lw 1 pt 5
#set style line 6 lt 1 lc rgb "orange" lw 1 pt 5

set size 1.0,1.0

set output "visualization.eps"

plot "data.dat" using 1:2 axes x1y1 with lines \
     ls 1 title "{/Arial=14 Sample Values}",\
     "data.dat" using 1:3 axes x1y1 with lines \
     ls 2 title "{/Arial=14 Sample Avg}",\
     "data.dat" using 1:5 axes x1y1 with lines \
     ls 3 title "{/Arial=14 VWC}"#,\
#     "data.dat" using 3:7 axes x1y1 with lines \
#     ls 4 title "{/Arial=14 green LED}",\
#     "data.dat" using 3:9 axes x1y2 with lines \
#     ls 5 title "{/Arial=14 energy consumed}",\
#     "data.dat" using 3:4 axes x1y1 with lines \
#     ls 6 title "{/Arial=14 CPU}"
     
