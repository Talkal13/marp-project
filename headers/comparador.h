

template <class K>
struct comparador_aristas {
    bool operator() (const K &a,const K &b) const {
        if (a.valor < b.valor) return true; //Ordenar por valor de la arista
        else if (a.valor > b.valor) return false;
        
        if (a.vertices.first < b.vertices.first) return true;
        else if (a.vertices.first > b.vertices.first) return false;

        if (a.vertices.second < b.vertices.second) return true;
        else if (a.vertices.second > b.vertices.second) return false;

        return true;
    }
};