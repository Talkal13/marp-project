#include <set>
#include <unordered_map>
#include "arista.h"
#include "comparador.h"
#include <vector>
#include "particion.h"

template <class T, class V>
using set_ordenado_aristas = std::set<arista<T, V>, comparador_aristas<arista<T, V>>>;


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

        std::vector<arista<T, V>> kruskal_A() {
            std::vector<arista<T, V>> sol;
            particion<T> _par;
            for (auto it = _vertices.begin(); it != _vertices.end(); ++it) {
                _par.incluir_elemento(*it);
            }
            auto it = _aristas.begin();
            while (sol.size() < _vertices.size() - 1) {
                std::pair<T, T> a = (*it).vertices;
                T rep_a = _par.encontrar_set(a.first);
                T rep_b = _par.encontrar_set(a.second);
                if (rep_a != rep_b) {
                    _par.unir_set(rep_a, rep_b);
                    sol.push_back(*it);
                }
                ++it;
            }
            return sol;
        }

        bool insertar_arista(arista<T, V> arista) {
            _vertices.insert(arista.vertices.first);
            _vertices.insert(arista.vertices.second);

            _aristas.insert(arista);

            return true;
        }

        friend std::ostream& operator<<(std::ostream& out, const grafo_kruskal& gk) {
            out << "Aristas: ";
            for (auto it = gk._aristas.begin(); it != gk._aristas.end(); ++it) {
                out << *it << " ";
            }
            out << std::endl << "Vertices: " << std::endl;
            for (auto it_v = gk._vertices.begin(); it_v != gk._vertices.end(); ++it_v) {
                out << *it_v << std::endl;
            }
            out << std::endl;
            return out;
        }



    private:
        set_ordenado_aristas<T, V> _aristas;
        std::set<T> _vertices;

};