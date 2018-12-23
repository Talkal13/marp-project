#ifndef NODE_H
#define NODE_H

#include <vector>

template <class T>
class Node {
    public:
        Node(T node, int index) {
            _node = node;
            _index = index;
        }
        ~Node() {}

        void add_edge(Node<T> *edge) {
                _edges.push_back(edge);
        }

        T _node;
        int _index;
        std::vector<Node<T>*> _edges;

};

#endif