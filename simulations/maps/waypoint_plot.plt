set terminal pdfcairo enhanced color \
   dashed dashlength 3.0 linewidth 1.0 butt \
   font "Times New Roman,18" size 4,4

set encoding utf8
set style increment user
set key font 'Roboto,8'
set ylabel "Y [m]"
set xlabel "X [m]"
set ytics 100
set xtics 100
set yrange [0:1000]
set xrange [0:1000]
unset key
set style line 1 lc rgb "#000000" pt 5 ps 0.5
set output "map1.pdf"
plot "2000_1000x1000_0.75_7.map" using 1:2 title 'Waypoint map' with points
set output "map2.pdf"
plot "2000_1000x1000_0.75_2.map" using 1:2 title 'Waypoint map' with points
set output "map3.pdf"
plot "2000_1000x1000_0.75_3.map" using 1:2 title 'Waypoint map' with points
set output "map4.pdf"
plot "2000_1000x1000_0.75_4.map" using 1:2 title 'Waypoint map' with points
set output "map5.pdf"
plot "2000_1000x1000_0.75_5.map" using 1:2 title 'Waypoint map' with points

