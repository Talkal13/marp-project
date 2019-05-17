#pragma once

#include <vector>
#include <map>
#include <set>
#include "../headers/sets.h"
/**
 * 
 * Undirected Graph
 * 
 * Only the min node handles the edge;
 * O(log n) search
 * 
 * 
 */ 
template <class T = int>
class graph {
    private:
        std::map<T, std::set<T>> _map;

        // Metadata
        int _nodes;
        int _edges;
        std::set<T> _V;
    public: 

    

        // Constructors and Destructors
        graph(){
            _nodes = 0;
            _edges = 0;
        }

        graph(const graph<T> &copy) {
            _map = copy._map;
            _nodes = copy._nodes;
            _edges = copy._edges;
            _V = copy._V;
        }

        graph(std::pair<T, std::set<T>> element) {
            add_node(element.first);
            for (T edge : element.second) {
                add_edge(std::make_pair(element.first, edge));
            }
        }

        graph(const graph<T> &G, std::set<T> subcomponents) {
            _nodes = 0;
            _edges = 0;
            _V = subcomponents;
            for (T v : subcomponents) {
                add_edges(std::make_pair(v, G.s_edges(v) / subcomponents));
            }
        }

        ~graph(){
        }

        void add_node(T key) {
            std::set<T> edges;
             
            if (_map.insert(std::make_pair(key, edges)).second) { 
                _nodes++;
                _V.insert(key);
            }
        }

        
        
        
        void add_edge(std::pair<T,T> tuple) {

            // Add nodes to the map
            add_node(tuple.first);
            add_node(tuple.second);

            if (tuple.first == tuple.second) return;

            T min = std::min(tuple.first, tuple.second);
            T max = std::max(tuple.first, tuple.second);

            // Only add the edge to the min node
            bool inserted = _map[min].insert(max).second;

            if (inserted) {
                _edges++;
            }
        }

        void add_edges(std::pair<T, std::set<T>> tuple) {
            add_node(tuple.first);
            for (T element : tuple.second) {
                add_edge(std::make_pair(tuple.first, element));
            }
        }

        /* Getters */

        int car_nodes() const {
            return _nodes;
        }

        int car_edges() const {
            return _edges;
        }

        int size() const {
            return car_nodes();
        }

        bool find(T key) {
            return _map.find(key) != _map.end();
        }

        std::set<T> edges(T key) const {
            std::set<T> edges = _map.at(key);
            for (std::pair<T, std::set<T>> pair : _map) {
                if (pair.first != key) {
                    if (pair.second.find(key) != pair.second.end()) edges.insert(pair.first);
                }
            }
            return edges;
        }

        std::set<T> s_edges(T key) const {
            return _map.at(key);
        }

        std::set<T> edges(T key, graph<T> G_prime) {
            std::set<T> edges = _map.at(key);
            for (std::pair<T, std::set<T>> pair : _map) {
                if (pair.first != key && G_prime.find(pair.first)) {
                    if (pair.second.find(key) != pair.second.end()) edges.insert(key);
                }
            }
            return edges;
        }

        bool exist_edge(T v, T u) {
            std::set<T> s = _map.at(v);
            return s.find(u) != s.end();
        }

        std::set<T> V() const {
            return _V;
        }


        /* Iterator */

        typedef typename std::map<T, std::set<T>>::iterator iterator;

        inline iterator begin()  { return _map.begin(); }

        inline iterator end()  { return _map.end(); }

        typedef typename std::map<T, std::set<T>>::const_iterator const_iterator;

        inline const_iterator cbegin() const { return _map.cbegin(); }

        inline const_iterator cend() const { return _map.end(); }

        typedef typename std::map<T, std::set<T>>::reverse_iterator reverse_iterator;

        inline reverse_iterator rbegin() { return _map.rbegin();}

        inline reverse_iterator rend() { return _map.rend();}

        /* Operators */

        std::set<T> operator[](T key) {
            return _map.at(key);
        }

        graph<T> operator-(T key) {
            graph<T> result;
            typename graph<T>::iterator it = begin();
            for (; it != end(); ++it) {
                T node = (*it).first;
                if (node != key) {
                    result.add_node(node);
                    typename std::set<T>::iterator edges = (*it).second.begin();
                    for (; edges != (*it).second.end(); ++edges) {
                        if (*edges != key) result.add_edge(std::make_pair(node, *edges));
                    }
                }
                
            }
            return result;
        }

        graph<T> operator+(std::pair<T, std::set<T>> pair) {
            graph<T> result = *this;
            for (T edge : pair.second) {
                result.add_edge(std::make_pair(pair.first, edge));
            }
            return result;
        }

        friend std::ostream& operator<<(std::ostream& out, const graph<T> &g) {
            typename graph<T>::const_iterator it = g.cbegin();
            for (; it != g.cend(); ++it) {
                out << (*it).first << ": ";
                typename std::set<T>::const_iterator edges = (*it).second.cbegin();
                for (; edges != (*it).second.cend(); ++edges) {
                    out << *edges << " ";
                }
                out << std::endl;
            }
            return out;
        }
        
};