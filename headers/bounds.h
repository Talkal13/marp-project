
#pragma once
#include <set>
#include "../headers/structs.h"
#include "../headers/globals.h"
#include <numeric>

/**
 * 
 * Bounds: level 0
 * No bounds - the optimistic is always the bigger posible and the pesimistic is always 0
 * 
 */


int optimistic_bound_0(std::vector<int> S, std::vector<double> C, std::vector<double> V, int E) {
    return 0;
}


int pesimistic_bound_0(std::vector<int> S, std::vector<double> C, std::vector<double> V, int E) {
    return V.size();
}

/**
 * 
 * Bounds: level 1
 * 
 * Simple bound
 * Optimistic: The bare minimun -> Total Volume / Volume of each box
 * 
 */


int optimistic_bound_1(std::vector<int> S, std::vector<double> C, std::vector<double> V, int E) {
    return 0;
}

int pesimistic_bound_1(std::vector<int> S, std::vector<double> C, std::vector<double> V, int E) {
    return 0;
}

/**
 * 
 * Bounds level 2
 * Complex bound
 * Optimistic: Paints the nodes os P so Clique size must be at most the number of color classes;
 *  
 */


int optimistic_bound_2(std::vector<int> S, std::vector<double> C, std::vector<double> V, int E) {
    return C.size();
}


int pesimistic_bound_2(std::vector<int> S, std::vector<double> C, std::vector<double> V, int E) {
    return C.size();
}