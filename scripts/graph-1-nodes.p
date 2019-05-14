set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title "Grafica del archivo ".filename
set xlabel "# Nodos"
set ylabel "Tiempo (s)"
set term png
set output "../graphs/".filename."-nodos.png"
set datafile missing NaN
plot    '../out/' . filename . '.dat' using 1:2 title 'Nodo' with linespoints smooth unique, \