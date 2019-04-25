#include <vector>
#include <set>

template <class T>
class Graph {
    public:
        // Constructors and Destructors
        Graph();
        ~Graph();

        void add_node(T, std::set<T> _edges);


        set::set<T> V;
    
    private:
        std::vector<T> _set;
        set::set<T> V;
}