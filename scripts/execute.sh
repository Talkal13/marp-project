if [[ -d $1 ]]; then
    for graph in $1/*; do
        echo $graph >> ../out/$2
        timeout 400s ../bin/pracmarp -f $graph -o ../out/$2 -b 2
    done
fi

