if [[ -d $1 ]]; then
    for d in $1/*; do
        if [[ -d $d ]]; then
            for graph in $d/*; do
                ../bin/main $graph ../out/graph-rantree.dat
            done
        fi
    done
    gnuplot -c graph.p -p
fi

