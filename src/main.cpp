#include "../headers/digraph.h"

int main() {
    digraph<int> p;
    int x[] = {1, 2, 3};
    int y[] = {4, 5};        
    p.add_edges(0, 3, x);
    p.add_edges(1, 2, y);
}