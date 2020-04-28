

template <class T>
class paint_nodes {
    private:
        std::vector<std::set<T>> _C;
    public:
        paint_nodes(graph<T> g) {
           it_paint_nodes(g);
        }

        /**
         * Generates all the color classes for a graph g
         * 
         */
        void it_paint_nodes(graph<T> g) {
            bool inserted = false, found = false;
            typename graph<T>::reverse_iterator it = g.rbegin();
            for (; it != g.rend(); ++it) { // For all v in V
                inserted = false;
                typename std::vector<std::set<T>>::iterator v_it = _C.begin();
                for (; v_it != _C.end(); ++v_it) { // For all existing color
                    found = false;
                    for (T v : (*it).second) { // For all edges in v
                        if ((*v_it).find(v) != (*v_it).end()) {found = true; break;}
                    }
                    if (!found) {
                        (*v_it).insert((*it).first); // If no edge exists then add to the same color
                        inserted = true;
                        break;
                    }
                }
                if (!inserted) {
                    std::set<T> n; // If an edge exist in every color then add a new color and push the candidate in
                    n.insert((*it).first);
                    _C.push_back(n);
                }
            }
        }

        int get_colors() {return _C.size();}
};