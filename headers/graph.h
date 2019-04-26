#pragma once

#include <vector>
#include <unordered_map>
#include <set>

template <class T>
class graph {
    public:
        // Constructors and Destructors
        graph();
        ~graph();

        void add_node(T);
        
        void add_edge(std::pair<T,T>);

        std::vector<T> operator[](T key);
        
    
    private:
        std::unordered_map<T, std::vector<T>> _map;
}