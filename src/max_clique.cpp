#include "../headers/max_clique.h"

/*
template <class T>
std::vector<T> matrix_max_clique(graph<T> G) {
    std::vector<Node<T>> solution;
    std::vector<std::vector<T>> matrix
    if (is_solution(G)) {
        return solution;
    }
    
}
*/

/**
 * Precondition: G = (V, E) is undirected and |V| > 0
 * 
 * 
 *
template <class T>
std::vector<T> matrix_max_clique(graph<T> G, int level) {
    // Found the solution ~ Base case = 1 node
    if (is_solution(G))
        return G[0];

    for (int i = 0; i < G.size(); i++) {
        if (is_solution(G - i)) return (G - i).V    
    }
    


} */


/**
 * Precondition: G = (V, E) is undirected and represented as a Matrix
 * 
 * 
 * Cost: O(|V| * (|V| - 1)) ~ O(N^2)
 */
template <class T>
bool is_solution(graph<T> G) {
    typename graph<T>::iterator it = G.begin();
    for (; it != G.end(); ++it) {
        typename std::vector<T>::iterator edges = *it.begin();
        typename graph<T>::iterator nodes = it;
        for (++nodes; edges != *it.end(); ++edges, ++nodes) {
            if (*nodes != *edges) return false;
        }
    }
    return true;
}

/**
 * Precondition Cyclid(G = (V,E))
 * 
 * Branch and Bound Binary algorithm to find the maximun size clique of a Graph G
 * 
 */

template <class T>
std::set<T> bnb_max_clique(graph<T> G) {
    if (is_solution(G)) return G.min().second;
}


template <class T>
void bnb_max_clique(graph<T> G, std::vector<T> &solution, int i) {
	
}