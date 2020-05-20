
#pragma once
#include <vector>
#include "../headers/structs.h"
#include "../headers/globals.h"
#include <numeric>
#include <math.h>
#include <algorithm>

/**
 * 
 * Bounds: level 0
 * No bounds - the optimistic is always the bigger posible and the pesimistic is always 0
 * 
 */


unsigned optimistic_bound_0(std::vector<int> S, std::vector<double> C, std::vector<double> V, double E) {
    return 1;
}


unsigned pesimistic_bound_0(std::vector<int> S, std::vector<double> C, std::vector<double> V, double E) {
    return V.size();
}

/**
 * 
 * Bounds: level 1
 * 
 * Simple bound
 * Optimistic: The bare minimun -> Overall pendent space / Volume of each box
 * Pesimistic: NextFit -> The object is inserted in the first bin available
 * 
 */


unsigned optimistic_bound_1(std::vector<int> S, std::vector<double> C, std::vector<double> V, double E) {
    return C.size();
}

unsigned pesimistic_bound_1(std::vector<int> S, std::vector<double> C, std::vector<double> V, double E) {
    return V.size();
}

/**
 * 
 * Bounds level 2
 * Complex bound
 * Optimistic: Paints the nodes os P so Clique size must be at most the number of color classes;
 *  
 */


unsigned optimistic_bound_2(std::vector<int> S, std::vector<double> C, std::vector<double> V, double E) {
    double pend = std::accumulate(V.begin() + S.size(), V.end(), 0);
    double hueco = std::accumulate(C.begin(), C.end(), 0);
    int x = int(ceil((pend - hueco) / E));
    return C.size() + std::max(0, x);
}


unsigned pesimistic_bound_2(std::vector<int> S, std::vector<double> C, std::vector<double> V, double E) {
    std::sort(V.begin(), V.end());
    unsigned pes = C.size();
    for (unsigned i = S.size(); i < V.size(); i++) {
        unsigned j = 0;
        while (C.size() > j && V[i] > C[j]) j += 1;  
        if (C.size() <= j) C.push_back(E - V[i]);
        else C[j] = C[j] - V[i];
        pes = std::max(pes, j);
    }
    return pes;
}