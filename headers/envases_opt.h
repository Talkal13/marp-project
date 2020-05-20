
#pragma once
#include "./globals.h"
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>

class node_e {
    public:

    std::vector<int> S;
    std::vector<double> C;
    unsigned upper_bound = 0;
    unsigned lower_bound = 0;

    node_e() {
        S = std::vector<int>();
        C = std::vector<double>();
    }

    node_e(std::vector<int> s, std::vector<double> c) {
        S = s;
        C = c;
    }

    void calculate(const std::vector<double> &V, double E) {
        upper_bound = (*bound.opt)(S, C, V, E);
        lower_bound = (*bound.pes)(S, C, V, E);
    }

};

std::vector<int> brb_envases(double E, std::vector<double> V) {
    

    // Sort the elements
    auto cmp = [](node_e left, node_e right) {return left.upper_bound > right.upper_bound || 
    (left.upper_bound == right.upper_bound && left.lower_bound < right.lower_bound);}; // Lambda function for comparations
    
    std::priority_queue<node_e, std::vector<node_e>, decltype(cmp)> queue(cmp);

    std::vector<int> best_sol; 
    node_e X,Y;

    X.S.push_back(0);
    X.C.push_back(E - V[0]);
    X.calculate(V, E);
    queue.push(X);

    double total = std::accumulate(V.begin(), V.end(), 0);
    unsigned optimo = total/E;
    for (unsigned i = 0; i < V.size(); i++) best_sol.push_back(i);
    unsigned best_cost = V.size();
    unsigned worst_posible = V.size();

    while (!queue.empty() && queue.top().C.size() <= worst_posible) {
        X = queue.top(); queue.pop();
        Y.S = X.S;
        Y.C = X.C;
        unsigned i = 0;
        marks.nodes++; // bechmark nodes
        clock_t begin = clock(); // start
        while (i <= X.C.size() && X.S.size() < V.size()) { //Try every possible combination;

            int j = X.S.size(); // Next object;

            if (Y.C.size() <= i) Y.C.push_back(E); //If the container does not exist make a new container

            //If there is space for the jth object in the ith container
            if (Y.C[i] >= V[j]) {
                
                Y.S.push_back(i); // Set the jth object to the ith container 
                Y.C[i] = Y.C[i] - V[j]; // Reduce the space of the ith container by the volume of the jth object;
                // If Y is solution
                if (Y.S.size() == V.size() && Y.C.size() < best_cost) { // If is a possible solution and is the best solution yet
                    best_sol = X.S; // Set the solution to set
                    best_cost = X.C.size();
                    if (best_cost == optimo) return best_sol;
                }
                else {
                    Y.calculate(V, E);
                    queue.push(Y);
                    worst_posible = std::min(best_cost, Y.lower_bound);
                }
                Y.S = X.S;
                Y.C = X.C;
            }
            i++;
        }
        clock_t end = clock();
        marks.avg_clocks_node += end - begin;
        

    }
    return best_sol;

}


