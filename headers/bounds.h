
#pragma once
#include <set>
#include "../headers/structs.h"
#include "../headers/globals.h"
#include "../headers/sets.h"
#include "../headers/paint.h"

/**
 * 
 * Bounds: level 0
 * No bounds - the optimistic is always the bigger posible and the pesimistic is always 0
 * 
 */

template <class T>
int optimistic_bound_0(std::set<T> C, std::set<T> P, graph<T> g) {
    return g.V().size();
}

template <class T>
int pesimistic_bound_0(std::set<T> C, std::set<T> P, graph<T> g) {
    return 0;
}

/**
 * 
 * Bounds: level 1
 * 
 * Simple bound
 * Optimistic: Assumes all candidates of P are part of the solution
 * 
 */

template <class T>
int optimistic_bound_1(std::set<T> C, std::set<T> P, graph<T> g) {
    return C.size() + P.size();
}

template <class T>
int pesimistic_bound_1(std::set<T> C, std::set<T> P, graph<T> g) {
    return C.size();
}

/**
 * 
 * Bounds level 2
 * Complex bound
 * Optimistic: Paints the nodes os P so Clique size must be at most the number of color classes;
 *  
 */

template <class T>
int optimistic_bound_2(std::set<T> C, std::set<T> P, graph<T> g) {
    return C.size() + paint_nodes<T>(graph<T>(g, P)).get_colors();
}

template <class T>
int pesimistic_bound_2(std::set<T> C, std::set<T> P, graph<T> g) {
    return C.size();
}