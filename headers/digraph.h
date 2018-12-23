#pragma once

#include <unordered_map>
#include <stack>
#include <vector>
#include <iostream>
#include "node.h"
#include <algorithm>


template <class T>
class digraph {
	public:
		digraph() {}
		~digraph() {
		}

		void add_node(T key) {
			Node<T> node(key, _map.size());
			_map.insert(std::pair<T, Node<T>>(key, node));
		}

		void add_edges(T key, std::vector<T> edges) {
			add_node(key);
			for (int i = 0; i < edges.size(); i++) {
				add_node(edges.at(i));
				_map.at(key).add_edge(&_map.at(edges.at(i)));
			}
				
		}

		bool isCyclic() {
			int ticks = 0;
			std::stack<Node<T>*> S;
			int index[_map.size()];
			init_struct<int>(index, _map.size(), -1);
			bool stack[_map.size()];
			init_struct<bool>(stack, _map.size(), false);
			bool x = false;
			typename std::unordered_map<T, Node<T>>::iterator it; 
			for (it = _map.begin(); it != _map.end(); ++it) {
				if (index[it->second._index] == -1) {
					x |= dfs_cycle(&it->second, ticks, S, index, stack);
				}
					
			}
			return x;
		}

		bool dfs_topological_sort(std::vector<Node<T>> &list) {
			//std::vector<Node<T>> list;
			bool red[_map.size()];
			init_struct<bool>(red, _map.size(), false);
			bool black[_map.size()];
			init_struct<bool>(black, _map.size(), false);
			bool x = true;
			typename std::unordered_map<T, Node<T>>::iterator it; 
			for (it = _map.begin(); it != _map.end(); ++it) {
				if (!black[it->second._index]) {
					x &= dfs_topological_sort(&it->second, list, red, black);
				}
					
			}
			return x;
		}

		std::vector<std::vector<Node<T>>> tarjan() {
			int ticks = 0;
			std::stack<Node<T>*> S;
			int index[_map.size()];
			init_struct<int>(index, _map.size(), -1);
			int lowlink[_map.size()];
			init_struct<int>(lowlink, _map.size(), -1);
			bool stack[_map.size()];
			init_struct<bool>(stack, _map.size(), false);
			std::vector<std::vector<Node<T>>> components;
			typename std::unordered_map<T, Node<T>>::iterator it; 
			for (it = _map.begin(); it != _map.end(); ++it) {
				if (index[it->second._index] == -1) {
					strongconnect(&it->second, ticks, S, index, lowlink, stack, components);
				}
					
			}
			return components;
		}


	private:

		template <typename A>
		void init_struct(A *index, int size, A init) {
			for (int i = 0; i < size; i++) {
				index[i] = init;
			}
		}

		bool dfs_topological_sort(Node<T> *v, std::vector<Node<T>> &list, bool red[], bool black[]) {
			if (black[v->_index]) return true;
			if (red[v->_index]) return false;
			red[v->_index] = true;
			for (int i = 0; i < v->_edges.size(); i++) {
				Node<T> *w = v->_edges.at(i);
				dfs_topological_sort(w, list, red, black);
			}
			black[v->_index] = true;
			typename std::vector<Node<T>>::iterator it = list.begin();
			list.insert(it, *v);
			return true;
		}

		bool dfs_cycle(Node<T> *v, int &ticks, std::stack<Node<T>*> &S, int index[], bool stack[]) {
			index[v->_index] = ticks;
			//v->_lowlink = index;
			ticks += 1;
			S.push(v);
			stack[v->_index] = true;

			for (int i = 0; i < v->_edges.size(); ++i) {
				Node<T> *w = v->_edges.at(i);
				if (index[w->_index] == -1) {
					return dfs_cycle(w, ticks, S, index, stack);
					//v->_lowlink = std::min(v->_lowlink, w->_lowlink);
				}
				else if (stack[w->_index]) {
					//v->_lowlink = std::min(v->_lowlink, w->_index);
					stack[v->_index] = false;
					S.pop();
					return true;
				}
			}

			stack[v->_index] = false;
			S.pop();

			return false;
		}

		void strongconnect(Node<T> *v, int &ticks, std::stack<Node<T>*> &S, int index[], int lowlink[], bool stack[], std::vector<std::vector<Node<T>>> &components) {
			int node = v->_index;
			index[node] = ticks;
			lowlink[node] = ticks;
			ticks += 1;
			S.push(v);
			stack[node] = true;

			for (int i = 0; i < v->_edges.size(); ++i) {
				Node<T> *w = v->_edges.at(i);
				if (index[w->_index] == -1) {
					strongconnect(w, ticks, S, index, lowlink, stack, components);
					lowlink[node] = std::min(lowlink[node], lowlink[w->_index]);
				}
				else if (stack[w->_index]) {
					lowlink[node] = std::min(lowlink[node], index[w->_index]);
				}
			}

			//check if node has no edges aka no posibility of being connected
			if (v->_edges.size() == 0) return;
			
			if (lowlink[node] == index[node]) {
				Node<T> *w;
				std::vector<Node<T>> component;
				do {
					w  = S.top();
					S.pop();
					stack[w->_index] = false;
					component.push_back(*w);
				} while (w->_index != v->_index);
				components.push_back(component);
			}
			return;	
		}
		

        std::unordered_map<T, Node<T>> _map;
};