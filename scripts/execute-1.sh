
../bin/pracmarp -d -n $1 -o ../out/$2.dat
gnuplot -e "filename='$2'" graph-1.p 
