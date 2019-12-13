
#include <unordered_map>

template <class T>
struct nodo_particion {
    T dato;
    nodo_particion *p;
    unsigned int rango;
};

template <class T>
class particion {
    public:

    particion() {}
    particion(T x) {
        incluir_elemento(x);
    }

    ~particion() {
        for (auto it = _nodos.begin(); it != _nodos.end(); ++it)
            delete (*it).second;
    }

    bool incluir_elemento(T x) {
        nodo_particion<T> *nodo = new nodo_particion<T>;
        nodo->dato = x;
        nodo->p = nodo;
        nodo->rango = 0;
        _nodos.insert({x, nodo});
        return true;
    }

    bool unir_set(T x, T y) {
        return juntar(encontrar_set(x), encontrar_set(y));
    }

    T encontrar_set(T x) {
        nodo_particion<T> *x_nodo = _nodos.at(x);
        if (x_nodo->p == x_nodo) return x;
        while (x_nodo->p != x_nodo) {
            x_nodo = x_nodo->p;
        }
        return x_nodo->dato;
    }

    bool juntar(T a, T b) {
        nodo_particion<T> *x = _nodos.at(a);
        nodo_particion<T> *y = _nodos.at(b);
        if (x->rango > y->rango) {
            y->p = x;
        }
        else {
            x->p = y;
            if (x->rango == y->rango) y->rango = y->rango + 1;
        }
        return true;
    }

    private:

    std::unordered_map<T, nodo_particion<T>*> _nodos;

};





