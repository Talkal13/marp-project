#pragma once

#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include "node.h"



template <class T>
class digraph {
	public:
		digraph() {}
		~digraph() {
		}

		void add_node(T key) {
			//Node<T> node(key);
			_map.insert(std::pair<T, Node<T>>(key, Node<T>(key)));
		}

		void add_edges(T key, int size, T edges[]) {
			if (_map.count(key) == 0) add_node(key);
			for (int i = 0; i < size; i++)
				_map[key].add_edge(&_map[edges[i]]);
		}

		bool hasCycle() {
			std::stack<T> calls;
			typename std::map<T, Node<T>>::iterator it; 
			for (it = _map.begin(); it != _map.end(); ++it) {
				if (!it->second->_black) {
					it->second->_red = true;
					it->second->_black = true;
					add_edges_to_call_stack(calls, it->second);
					while(!calls.empty()) {
						Node<T> *x = calls.top();
						calls.pop();
						if (x->_red) return true;
						if (!x->_black) {
							add_edges_to_call_stack(calls, x);
							x->_red = true;
							x->_black = true;
						}
						 
					}
				}
			}
		}

	private:
		void add_edges_to_call_stack(std::stack<T> &calls, Node<T> t) {
			typename std::vector<Node<T>*>::iterator vit;
			for (vit = t._edges.begin(); vit !=  t._edges.end(); ++vit);
				calls.push(*vit);
		}
        std::map<T, Node<T>> _map;
};