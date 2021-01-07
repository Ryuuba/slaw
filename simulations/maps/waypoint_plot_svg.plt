set terminal svg size 500,500 dynamic enhanced

set style increment user
set key font ',8'
set ylabel "Y [m]"
set xlabel "X [m]"
set ytics 10
set xtics 10
set yrange [0:200]
set xrange [0:200]
unset key
set style line 1 lc rgb "#000000" pt 5 ps 0.5
set output "small_200x200.svg"
plot "small_200x200.map" using 1:2 title 'Waypoint map' with points
