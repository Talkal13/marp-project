
#include "../headers/graph.h"
#include <unordered_map>
#include <set>
#include <map>


template <class T>
class dinamic_clique {
    private:
        std::map<std::set<T>, bool> _map;
        graph<T> _g;
    public:

        dinamic_clique(graph<T> G) {
            _map.insert({std::set<T>(), true});
            for (std::pair<T, std::set<T>> element : G) {
                std::set<T> foo;
                foo.insert(element.first);
                _map.insert(std::make_pair(foo, true));
            }

            _g  = G;
        }

        bool check_clique(std::pair<std::set<T>, T> set) {
            
            std::set<T> original;
            for (T element : set.first) {
                original.insert(element);
            }
            bool result = true;

            if (!_map.at(std::set<T>(original))) result = false;

            if (result) {
                for (T edge : original) {
                    std::set<T> edges = _g.edges(set.second);
                    if (edges.find(edge) == edges.end()) {result = false; break;}
                }
            }
            original.insert(set.second);
            _map.insert(std::make_pair(std::set<T>(original), result));
            return result;
            
        }

        bool check_clique(std::set<T> set) {
            return _map.at(set);
        }

        friend std::ostream& operator<<(std::ostream& out, const dinamic_clique<T> &d) {
            for (auto key_value : d._map) {
                for (auto key : key_value.first) {
                    out << key << " ";
                }
                out << ": " << key_value.second << std::endl;
            }
            return out;
        }
};