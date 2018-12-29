#include "../headers/digraph.h"
#include <string>
#include <sstream>
#include <utility>
#include <iterator>

using namespace std;

int main() {
    digraph<int> p;
    string line;
    while (getline(cin, line) || line == "") {
        istringstream this_line(line);
        istream_iterator<int> begin(this_line), end;
        vector<int> values(begin, end);
        int key = values.at(0);
        vector<int>::iterator i = values.begin();
        values.erase(i);
        p.add_edges(key, values);
    }
    
    std::vector<Node<int>> v;
    if (p.dfs_topological_sort(v)) {
        std::vector<std::vector<Node<int>>> vec = p.tarjan();
            for (int i = 0; i < vec.size(); i++) {
                cout << "Cycle number " << i << endl;
                for (int j = vec[i].size() - 1; j > 0; j--) {
                    cout << vec[i][j]._node << " -> ";
                }
                cout << vec[i][0]._node << endl;
            }
    }
    else {
            std::vector<Node<int>>::iterator it;
            for (int i = 0; i < v.size() -1 ; i++) {
                cout << (v.at(i))._node << " - ";
            }
            cout << v.at(v.size() - 1)._node << endl;
    }
    
    
}