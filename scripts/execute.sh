if [[ -d $1 ]]; then
    for graph in $1/*; do
        ../bin/pracmarp -f $graph -o ../out/$2.dat
    done
    gnuplot -e "filename='$2'" graph.p 
fi

