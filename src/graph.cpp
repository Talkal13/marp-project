#include "../headers/graph.h"

/**
 *
 * Add node to the map
 *
 * No effect if key is already in the _map ~ https://en.cppreference.com/w/cpp/container/unordered_map/insert
 * 
 */
template<class T>
void graph<T>::add_node(T key) {
    std::vector<T> edges;
    _map.insert(std::pair<T, std::vector<T>>(key, edges));
}

/**
 * Add edges only in the node with the min key
 * 
 * 
 */
template<class T>
void graph<T>::add_edge(std::pair<T,T> tuple) {

    // Add nodes to the map
    add_node(tuple.first);
    add_node(tuple.second);

    // Only add the edge to the min node
    (tuple.first < tuple.second) ? 
        _map[tuple.first].insert(tuple.second) :
        _map[tuple].insert(tuple.first);
}


template<class T>
std::vector<T> graph<T>::operator[](T key) {
    return _map.at(key);
}

template class graph<int>;