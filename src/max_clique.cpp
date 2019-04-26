#include <vector>
#include <priority_queue>
#include "headers/node.h"
#include "headers/digraph.h"
#include "headers/graph.h"


template <class T>
std::vector<T> matrix_max_clique(Graph<T> G) {
    std::vector<Node<T>> solution;
    std::vector<std::vector<T>> matrix
    if (is_solution(G)) {
        return G.V();
    }
    
}


/**
 * Precondition: G = (V, E) is undirected and |V| > 0
 * 
 * 
 */
std::vector<T> matrix_max_clique(Graph<T> G, int level) {
    // Found the solution ~ Base case = 1 node
    if (is_solution(G))
        return G.V

    for (int i = 0; i < G.size(); i++) {
        if (G.exist(i) && is_solution(G - i)) return (G - i).V    
    }
    


}


/**
 * Precondition: G = (V, E) is undirected and represented as a Matrix
 * 
 * 
 * Cost: O(|V| * (|V| - 1)) ~ O(N^2)
 */
bool is_solution(Graph<T> G) {
    for (int i = 0; i < G.size(); i++) {
        for (int j = i + 1; j < G.size(); j++) {
            if (!G[i][j]) return false;
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
std::vector<Node<T>> bnb_max_clique(digraph<T> G) {
    
         

}


template <class T>
void bnb_max_clique(Graph<T> G, std::vector<Node<T>> &solution, int i) {
	
}