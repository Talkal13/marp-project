#include <utility>

template <class T, class V> 
struct arista {
    std::pair<T, T> vertices;
    V valor;


};

template <class T, class V>
std::ostream& operator<<(std::ostream& out, const arista<T, V>& a) {
    out << "({" << a.vertices.first << ", " << a.vertices.second << "}, " << a.valor << ")";
    return out;
}

