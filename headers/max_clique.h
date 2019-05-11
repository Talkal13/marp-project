#pragma once

#include <vector>
#include <queue>
#include "../headers/node.h"
#include "../headers/digraph.h"
#include "../headers/graph.h"

typedef struct {
    int nodes = 0;
    clock_t avg_clocks_node = 0;
    clock_t complete_time = 0;
} benchmark;

template <class T>
std::set<T> operator -=(std::set<T> set1, std::set<T> set2) {
    std::set<T> result;
    for (T edge : set1) {
        for (T edge2 : set2) {
            if (edge == edge2) result.insert(edge);
        }
    }
    return result;
}

/**
 * Precondition: G = (V, E) is undirected and represented as a Matrix
 * 
 * 
 * Cost: O(|V| * (|V| - 1)) ~ O(N^2)
 */
template <class T>
bool is_solution(graph<T> G) {
    return G.car_edges() == G.car_nodes() * (G.car_nodes() - 1) / 2;
}

template <class T>
bool is_empty(graph<T> G) {
    return G.car_nodes() == 0;
}

template <class T>
int estimated_cost(graph<T> G) {
    return G.size();
}


template <class T>
int pesimist_cost(graph<T> G) {
    int cost = 0;
    typename graph<T>::iterator it = G.begin();
    for (; it != G.end(); ++it) {
        cost += (*it).second.size(); // Add all the edges
    }
    return (G.size() * (G.size() - 1) / 2) - cost;
}

template <class T>
int pesimist_cost_heavy(graph<T> G) {
    return G.independent().size();
}

/**
 * Precondition Cyclid(G = (V,E))
 * 
 * Branch and Bound Binary algorithm to find the maximun size clique of a Graph G
 * 
 */

template<class T>
std::set<T> bnb_max_clique(graph<T> G) {
    auto cmp = [](graph<T> left, graph<T> right) {return left.size() < right.size();}; //https://en.cppreference.com/w/cpp/container/priority_queue
    std::priority_queue<graph<T>, std::vector<graph<T>>, decltype(cmp)> queue(cmp);
    queue.push(G);
    int best_cost = pesimist_cost(G);
    while (!queue.empty()) { // No need to check for best cost because the top of the queue is always the best or equal that the sucesors
        graph<T> candidate = queue.top(); queue.pop();
        if (is_solution(candidate)) return candidate.V(); // No need to check other solutions -> Is going to be the best
        typename graph<T>::iterator it = candidate.begin();
        for (; it != candidate.end(); ++it) {
            T K_i = (*it).first;
            graph<T> K = candidate - K_i;
            if (pesimist_cost(K) <= best_cost) {// Is always compleateable 
                queue.push(K);
                best_cost = pesimist_cost(K);
            }
        }
    }
    return std::set<T>();
}

template <class T>
std::set<T> bnb_max_clique_benchmarks(graph<T> G, benchmark &marks) {
    auto cmp = [](graph<T> left, graph<T> right) {return left.size() < right.size();}; //https://en.cppreference.com/w/cpp/container/priority_queue
    std::priority_queue<graph<T>, std::vector<graph<T>>, decltype(cmp)> queue(cmp);
    queue.push(G);
    int best_cost = pesimist_cost(G);
    
    while (!queue.empty()) { // No need to check for best cost because the top of the queue is always the best or equal that the sucesors
        clock_t clock_node = clock();
        graph<T> candidate = queue.top(); queue.pop();
        marks.nodes++;
        if (is_solution(candidate)) {
            marks.avg_clocks_node += (clock() - clock_node);
            marks.avg_clocks_node /= marks.nodes;
            return candidate.V(); // No need to check other solutions -> Is going to be the best
        }
        typename graph<T>::iterator it = candidate.begin();
        for (; it != candidate.end(); ++it) {
            T K_i = (*it).first;
            graph<T> K = candidate - K_i;
            if (pesimist_cost(K) <= best_cost) {// Is always compleateable 
                queue.push(K);
                best_cost = pesimist_cost(K);
            }
        }
        marks.avg_clocks_node += (clock() - clock_node);
    }
    
    return std::set<T>();
}

template <class T>
std::set<T> bnb_increment_max_clique_benchmarks(graph<T> G, benchmark &marks) {
    
    auto cmp = [](graph<T> left, graph<T> right) {return left.size() < right.size();}; //https://en.cppreference.com/w/cpp/container/priority_queue
    
    std::priority_queue<graph<T>, std::vector<graph<T>>, decltype(cmp)> queue(cmp);
    int best_cost = pesimist_cost_heavy(G);
    
    for (std::pair<T, std::set<T>> element : G) {
        graph<T> g;
        g.add_node(element.first);
        queue.push(g);
        int best_cost = std::min(best_cost, pesimist_cost_heavy(g));
    }
    graph<T> candidate = queue.top();
    std::set<T> best_solution = candidate.V();
    
    while (!queue.empty()) {

        graph<T> candidate = queue.top(); queue.pop();
        if (is_solution(candidate) ^ candidate.size() > best_solution.size()) {
            
            best_solution = candidate.V();
        }
        graph<T> option;
        typename graph<T>::iterator it = candidate.begin();
        std::set<T> edges = G.edges((*it).first);
        for (; it != candidate.end(); ++it) {
            edges -= (*it).second; 
        }

    }
    
    return best_solution;
}
