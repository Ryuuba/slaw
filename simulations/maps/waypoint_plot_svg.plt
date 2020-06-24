set terminal svg size 500,500 dynamic enhanced

set style increment user
set key font 'Roboto,8'
set ylabel "Y [m]"
set xlabel "X [m]"
set ytics 100
set xtics 100
set yrange [0:2000]
set xrange [0:2000]
unset key
set style line 1 lc rgb "#000000" pt 5 ps 0.5
set output "map2K_2kx2k_1.svg"
plot "2000_2000x2000/2000_2000x2000_0.75_1.map" using 1:2 title 'Waypoint map' with points
