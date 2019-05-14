

#include <set>

/**
 * 
 * Bounds: level 0
 * 
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
 * 
 * 
 * 
 */
template <class T>
int optimistic_bound_2(std::set<T> C, std::set<T> P, graph<T> g) {
    return C.size() + P.size();
}

template <class T>
int pesimistic_bound_2(std::set<T> C, std::set<T> P, graph<T> g) {
    int solution = C.size();
    bool clique = true;
    for (T e : P) {
        for (T k : P) {
            if (!g.exist_edge(e, k)) clique = false;
        }
        if (clique) {
            solution++;
            clique = true;
        }
    }
    return solution;
}