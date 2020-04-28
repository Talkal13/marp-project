#pragma once

#include <vector>
#include <queue>
#include "../headers/graph.h"
#include "../headers/sets.h"
#include "../headers/structs.h"
#include "../headers/globals.h"



template <class T>
class node_u{
    public:
    std::set<T> C;
    std::set<T> P;
    int upper_bound = 0;
    int lower_bound = 0;

    node_u() {}

    node_u(std::set<T> S, graph<T>  g) {
        C = S;
        P = N(g, S);
        upper_bound = (*bound.opt)(C, P, g);
        lower_bound = (*bound.pes)(C, P, g);
    }
};


// Is solution if |E| = |V| * (|V| - 1) / 2
template <class T>
bool is_solution(std::set<T> solution, const graph<T> &g) {
    graph<T> sub_g(g, solution);
    return sub_g.car_edges() == sub_g.size() * (sub_g.size() - 1) / 2;
}

template <class T>
bool is_solution(const graph<T> &g) {
    return g.car_edges() == g.size() * (g.size() - 1) / 2;
}

template <class T>
bool is_empty(graph<T> G) {
    return G.car_nodes() == 0;
}

template <class T>
bool posible(std::set<T> solution, const graph<T> &g) {
    return is_solution(solution, g);
}

template <class T>
std::set<T> division_with_purge(std::set<T> S, std::set<T> C) {
    T lower = *(C.begin());
    std::set<T> result;
    typename std::set<T>::iterator it = S.find(lower);
    ++it;
    for (; it != S.end(); ++it) {
        if (C.find(*it) == C.end()) result.insert(*it);
    }
    return result;
}

template <class T>
std::set<T> N(graph<T> G, std::set<T> C) {
    std::set<T> P;
    for (T element : division_with_purge(G.V(), C)) {
        if (posible(C + element, G)) P.insert(element);
    }
    return P;
}



/**
 * Precondition Cyclid(G = (V,E))
 * 
 * Branch and Bound Binary algorithm to find the maximun size clique of a Graph G
 * 
 */


template <class T>
std::set<T> bnb_max_clique_benchmarked(graph<T> G) {

    auto cmp = [](node_u<T> left, node_u<T> right) {return left.upper_bound < right.upper_bound;}; // Lambda function for comparations
    
    std::priority_queue<node_u<T>, std::vector<node_u<T>>, decltype(cmp)> queue(cmp);

    if (is_solution(G)) return G.V(); // If is solution return the whole graph
    
    for (std::pair<T, std::set<T>> element : G) {
        queue.push(node_u<T>(make_set(element.first), G)); // Push the first candidates of the tree - the unitary sets
    }
    node_u<T> parent;
    std::set<T> best_solution = {};
    int best_cost = -1;
    
    while (!queue.empty() && parent.upper_bound > best_cost) {

        parent = queue.top(); queue.pop(); // Parent is the best candidate available
        if (is_solution(parent.C, G) && (int) parent.C.size() > best_cost) { //If is a better solution replace it.
                best_solution = parent.C;
                best_cost = parent.C.size(); 
        }
        marks.nodes++; // bechmark nodes
        clock_t begin = clock(); // start
        for (T v : parent.P) { // For every vertex in the set of posibles
            node_u<T> node = node_u<T>(parent.C + v, G);
            if (is_solution(node.C, G) && (int) node.C.size() > best_cost) { 
                best_solution = node.C; //If is solution replace
                best_cost = node.C.size();
            }

            if (node.upper_bound > best_cost) { // Is completable because every child in P is completable and is a plausible candidate
                queue.push(node);
                if (node.lower_bound > best_cost) 
                    best_cost = node.lower_bound;
            }
        }
        clock_t end = clock();
        marks.avg_clocks_node += end - begin;
    }
    return best_solution;
}