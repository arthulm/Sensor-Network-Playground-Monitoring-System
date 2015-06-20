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
#set style line 3 lt 1 lc rgb "red" lw 1 pt 3
#set style line 4 lt 1 lc rgb "yellow" lw 1 pt 4
#set style line 5 lt 1 lc rgb "black" lw 1 pt 5
#set style line 6 lt 1 lc rgb "orange" lw 1 pt 5

set size 1.0,1.0

set output "visualization.eps"

plot "data.dat" using 1:2 axes x1y1 with lines \
     ls 1 title "{/Arial=12 CO2}",\
     "data.dat" using 1:3 axes x1y1 with lines \
     ls 2 title "{/Arial=12 Average}"#,\
#     "data.dat" using 1:($4/10) axes x1y1 with lines \
#     ls 3 title "{/Arial=12 Temp/10}",\
#     "data.dat" using 1:5 axes x1y1 with lines \
#     ls 4 title "{/Arial=12 Con Humidity}",\
#     "data.dat" using 1:6 axes x1y1 with lines \
#     ls 5 title "{/Arial=12 Con Temp}"#,\
#     "data.dat" using 3:4 axes x1y1 with lines \
#     ls 6 title "{/Arial=14 CPU}"
     
