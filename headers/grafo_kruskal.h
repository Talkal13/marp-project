#include <set>
#include <unordered_map>
#include "arista.h"
#include "comparador.h"
#include <vector>

template <class V>
using arista_grafo = arista<unsigned int, V>;


/**
 * 
 * Implementacion de un grafo como una lista ordenada de aristas
 * 
 * Invariante:
 *      - El grafo ha de ser completo
 * 
 */
template <class T, class V>
class grafo_kruskal {
    public:

        grafo_kruskal() {}

        std::vector<arista<T, V>> kruskal() {
            std::vector<arista<T, V>> solucion;
            int con = 0;
            bool marca[_n] = {false};
            for (auto it = _aristas.begin(); it != _aristas.end(); ++it) {
                unsigned int v1 = it->vertices.first;
                unsigned int v2 = it->vertices.second;
                if (!marca[v1] && !marca[v2]) {
                    marca[v1] = true;
                    marca[v2] = true;
                    arista<T, V> a;
                    a.vertices = std::make_pair(_vn[v1], _vn[v2]);
                    a.valor = it->valor;
                    solucion.push_back(a);
                }
                if (solucion.size() == _n) return solucion;
            }
            return solucion;
        }

        bool insertar_arista(arista<T, V> arista) {
            arista_grafo<V> a;

            a.vertices = traducir_nv(arista.vertices);
            a.valor = arista.valor;

            _aristas.insert(a);

            return true;

        }

        friend std::ostream& operator<<(std::ostream& out, const grafo_kruskal& gk) {
            out << "Aristas: ";
            for (auto it = gk._aristas.begin(); it != gk._aristas.end(); ++it) {
                out << *it << " ";
            }
            out << std::endl << "Vertices: " << std::endl;
            for (unsigned int i = 0; i < gk._n; i++) {
                out << (gk._vn.at(i)) << std::endl;
            }
            out << std::endl;
            return out;
        }



    private:
        std::set<arista_grafo<V>, comparador_aristas<arista_grafo<V>>> _aristas;
        std::unordered_map<unsigned int, T> _vn; // Vertice a nodo
        std::unordered_map<T, unsigned int> _nv; // Nodo a vertice
        unsigned int _n = 0; // Contador de vertices



        std::pair<unsigned int, unsigned int> traducir_nv(std::pair<T, T> nodos) {
            unsigned int v1, v2;
            v1 = encuentra_y_asigna_nv(nodos.first);
            v2 = encuentra_y_asigna_nv(nodos.second);

            return std::make_pair(std::min(v1,v2), std::max(v1, v2));
        }

        unsigned int encuentra_y_asigna_nv(T nodo) {
            unsigned int v;
            if (_nv.find(nodo) == _nv.end()) {
                _nv.insert({nodo, _n});
                _vn.insert({_n, nodo});
                v = _n;
                _n++;
            } else {
                v = _nv.at(nodo);
            }
            return v;
        }

};