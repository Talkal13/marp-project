#ifndef NODE_H
#define NODE_H

#include <vector>

template <class T>
class Node {
    public:
        Node(T node) {
            _node = node;
            _black = false;
            _red = false;
        }
        Node() {}
        ~Node() {}

        void add_edge(Node<T> *edge) {
                _edges.push_back(edge);
        }

        T _node;
        bool _black;
        bool _red;
        std::vector<Node<T>*> _edges;

};

#endif