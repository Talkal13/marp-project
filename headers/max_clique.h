#pragma once

#include <vector>
#include <queue>
#include "../headers/graph.h"
#include "../headers/sets.h"
#include "../headers/dinamic_clique.h"


typedef struct {
    int nodes = 0;
    clock_t avg_clocks_node = 0;
    clock_t complete_time = 0;
} benchmark;

template <class T>
class node_u{
    public:
    std::set<T> C;
    std::set<T> P;
    int upper_bound = 0;
    int lower_bound = 0;

    node_u() {
        
    }

    node_u(std::set<T> S, dinamic_clique<T> &d, const graph<T> &G, int (*opt)(std::set<T>, std::set<T>, graph<T>), int (*pes)(std::set<T>, std::set<T>, graph<T>)) {
        C = S;
        P = N(d, G, S);
        upper_bound = (*opt)(C, P, G);
        lower_bound = (*pes)(C, P, G);
    }

};


/**
 * Precondition: G = (V, E) is undirected and represented as a Matrix
 * 
 * 
 * Cost: O(|V| * (|V| - 1)) ~ O(N^2)
 */
template <class T>
bool is_solution(dinamic_clique<T> &d, std::set<T> solution) {
    return d.check_clique(solution);
}

template <class T>
bool is_empty(graph<T> G) {
    return G.car_nodes() == 0;
}

template <class T>
bool posible(dinamic_clique<T> &d, std::pair<std::set<T>, T> solution) {
    return d.check_clique(solution);
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
std::set<T> N(dinamic_clique<T> &d, graph<T> G, std::set<T> C) {
    std::set<T> P;
    for (T element : division_with_purge(G.V(), C)) {
        if (posible(d, {C, element})) P.insert(element);
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
std::set<T> bnb_increment_max_clique_benchmarks(graph<T> G, benchmark &marks, int (*opt)(std::set<T>, std::set<T>, graph<T>), int (*pes)(std::set<T>, std::set<T>, graph<T>)) { //https://stackoverflow.com/questions/9410/how-do-you-pass-a-function-as-a-parameter-in-c
    
    dinamic_clique<T> d(G);

    auto cmp = [](node_u<T> left, node_u<T> right) {return left.lower_bound > right.lower_bound;}; //https://en.cppreference.com/w/cpp/container/priority_queue
    
    std::priority_queue<node_u<T>, std::vector<node_u<T>>, decltype(cmp)> queue(cmp);
    
    for (std::pair<T, std::set<T>> element : G) {
        queue.push(node_u<T>(make_set(element.first), d, G, opt, pes));
        
    }
    node_u<T> node;
    if (!queue.empty()) node = queue.top();
    std::set<T> best_solution = {};
    int best_cost = 0;
    
    while (!queue.empty() && node.upper_bound > best_cost) {

        node_u<T> parent = queue.top(); queue.pop();
        marks.nodes++;
        clock_t begin = clock();
        for (T v : parent.P) {
            node_u<T> node = node_u<T>(parent.C + v, d, G, opt, pes);
            if (is_solution(d, node.C) && node.C.size() > best_cost) { 
                best_solution = node.C;
                best_cost = node.C.size();
            }

            if (node.upper_bound > best_cost) { // Is completable because every child in P is completable;
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
