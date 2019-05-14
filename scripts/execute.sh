if [[ -d $1 ]]; then
    for graph in $1/*; do
        echo $graph
        ../bin/pracmarp -f $graph -o ../out/$2.dat
    done
    gnuplot -e "filename='$2'" graph-1.p 
fi

