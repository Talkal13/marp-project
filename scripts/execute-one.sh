if [[ -d $1 ]]; then
    for graph in $1/*; do
        ../bin/main $graph ../out/graph-$2.dat
    done
    gnuplot -p -e "filename='../out/graph-$2.dat'" graph.p
fi

