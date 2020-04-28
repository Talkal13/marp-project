#pragma once

#include <set>
#include <algorithm>

/**
 * 
 * Helper sets operations
 * 
 */

template <class T>
std::set<T> operator /(std::set<T> set1, std::set<T> set2) {
    std::set<T> result;
    typename std::set<T>::iterator it = set1.begin();
    for (; it != set1.end(); ++it) {
        if (set2.find((*it)) != set2.end()) result.insert((*it));
    }
    return result;
}

template <class T>
std::set<T> operator -(std::set<T> original, std::set<T> operand) {
    std::set<T> result;
    for (T edge : original) {
        if (operand.find(edge) == operand.end()) result.insert(edge);
    }
    return result;
}

template <class T>
std::set<T> operator -(std::set<T> s, T element) {
    std::set<T> result = s;
    result.extract(element);
    return result;
}

template <class T>
std::set<T> operator +(std::set<T> set1, std::set<T> set2) {
    std::set<T> result;
    for (T element : set1) result.insert(element);
    for (T element : set2) result.insert(element);
    return result;
}

template <class T>
std::set<T> operator +(std::set<T> set1, T element) {
    std::set<T> result = set1;
    result.insert(element);
    return result;
}

template <class T>
std::set<T> make_set(T element) {
    std::set<T> result;
    result.insert(element);
    return result;
}

template <class T>
bool operator==(std::set<T> one, std::set<T> other) { 
    if (one.size() != other.size()) return false;
    typename std::set<T>::iterator it = one.begin();
    typename std::set<T>::iterator it2 = other.begin();
    for (; it != one.end() && it2 != other.end(); ++it, ++it2) {
        if ((*it) != (*it2)) return false;
    }
    return true;
}
struct Comparator {
    template <class T>
    bool operator()(const std::set<T> &s, const std::set<T> &k) const {
        if (s.size() < k.size()) return true;
        else if (s.size() > k.size()) return false;
        typename std::set<T>::iterator it = s.begin();
        typename std::set<T>::iterator it2 = k.begin();
        for (; it != s.end() && it2 != k.end(); ++it, ++it2) {
            if ((*it) < (*it2)) return true;
            else if ((*it) > ((*it2))) return false;
        }
        return false;
    }
};
