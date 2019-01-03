# Gnuplot script file for plotting data in file "force.dat"
# This file is called   force.p
set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title "Grafica del archivo ".filename
set xlabel "# Nodos"
set ylabel "Tiempo (ms)"
set term png
set output "../graphs/".filename.".png"
set datafile missing NaN
plot    '../out/' . filename . '.dat' using 1:2 title 'Cycles' with linespoints smooth unique , \
    '../out/' . filename . '.dat' using 1:($3 == 0 ? 1/0  : $3) title 'Topological sort' with linespoints smooth unique, \
    '../out/' . filename . '.dat' using 1:($4 == 0 ? 1/0 : $4) title 'Strong Components' with linespoints smooth unique
