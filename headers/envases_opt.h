
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
    int upper_bound = 0;
    int lower_bound = 0;

    node_e() {}

    node_e(std::vector<int> s, std::vector<double> c) {
        S = s;
        C = c;
    }

    void calculate(std::vector<double> V, double E) {
        upper_bound = (*bound.opt)(S, C, V, E);
        lower_bound = (*bound.pes)(S, C, V, E);
    }

};

bool is_possible(std::vector<double> C, int i, std::vector<double> V, int j) {
    return C[i] >= V[j];
}

bool is_solution(std::vector<e_object> objects) {
    objects.size() == 0;
}

std::vector<int> brb_envases(double E, std::vector<double> V) {
    std::vector<int> best_sol; 
    node_e X,Y;

    // Sort the elements
    auto cmp = [](node_e left, node_e right) {return left.upper_bound > right.upper_bound;}; // Lambda function for comparations
    
    std::priority_queue<node_e, std::vector<node_e>, decltype(cmp)> queue(cmp);

    X.S.push_back(0);
    X.C.push_back(E - V[0]);
    X.calculate(V, E);
    queue.push(X);

    double total = std::accumulate(V.begin(), V.end(), 0);
    int optimo = total/E;
    bool found = false;
    std::vector<int> best_sol;
    for (int i = 0; i < V.size(); i++) best_sol.push_back(i);
    int best_cost = V.size();

    while (!queue.empty() && X.upper_bound < best_cost) {
        X = queue.top(); queue.pop();
        Y.S = X.S;
        Y.C = X.C;
        int i = 0;
        while (i <= X.C.size()) {
            // If is posible
            int j = X.S.size();

            // If Y is possible
            if (Y.C[i] >= V[j]) {
                
                Y.S.push_back(i); // Set the jth object to the ith container
                if (Y.C.size() < i) Y.C[i] = Y.C[i] - V[j]; // Reduce the space of the ith container by the volume of the jth object;
                else Y.C.push_back(E - V[j]);

                // If Y is solution
                if (Y.S.size() == V.size() && Y.C.size() < best_cost) { // If is a possible solution and is the best solution
                    best_sol = X.S; // Set the solution to set
                    best_cost = X.C.size();
                    if (best_sol.size() == optimo) return best_sol;
                }
                else {
                    Y.calculate();
                    queue.push(Y);
                }
                Y.S = X.S;
                Y.C = X.C;
            }
            i++;
        }

        

    }

}



/**
 * 
 * 
 */
std::set<int> envases_benchmarked(std::vector<e_object> objects, int E) {
    // Sort the elements
    auto cmp = [](node_u left, node_u right) {return left.upper_bound < right.upper_bound;}; // Lambda function for comparations
    
    std::priority_queue<node_u, std::vector<node_u>, decltype(cmp)> queue(cmp);

    std::vector<e_set_objects> candidate = std::vector<e_set_objects>();
    candidate.push_back(e_set_objects(objects[0])); // Set the first object to the first container
    objects.erase(objects.begin());

    queue.push(node_u(candidate, objects, E)); // Push the first candidates of the tree - the unitary sets
    node_u parent;
    std::vector<e_set_objects> best_solution;
    int best_cost = objects.size() + 1;
    
    while (!queue.empty() && parent.upper_bound > best_cost) {

        parent = queue.top(); queue.pop(); // Parent is the best candidate available
        if (is_factible(parent.C, E) && (int) parent.C.size() < best_cost) { //If is a better solution replace it.
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


