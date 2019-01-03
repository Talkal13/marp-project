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
		digraph() {
			n_nodes = 0;
			n_edges = 0;
		}
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

		bool has_node(T key) {
			return _map.find(key) != _map.end();
		}

		void add_edge(T key, T edge) {
			add_node(key);
			add_node(edge);
			_map.at(key).add_edge(&_map.at(edge));
		}

		/**
		 * Precondition: Acyclid(G(V, E)) 
		 * 
		 */ 
		std::stack<Node<T>> dfs_topological_sort() {
			std::stack<Node<T>> list;
			bool red[_map.size()];
			init_struct<bool>(red, _map.size(), false);
			bool black[_map.size()];
			init_struct<bool>(black, _map.size(), false);
			typename std::unordered_map<T, Node<T>>::iterator it; 
			for (it = _map.begin(); it != _map.end(); ++it) {
				if (!black[it->second._index]) {
					dfs_topological_sort(&it->second, list, red, black);
				}
					
			}
			return list;
		}

		bool dfs_search_cycles() {
			bool red[_map.size()];
			init_struct<bool>(red, _map.size(), false);
			bool black[_map.size()];
			init_struct<bool>(black, _map.size(), false);
			bool x = false;
			typename std::unordered_map<T, Node<T>>::iterator it; 
			for (it = _map.begin(); it != _map.end(); ++it) {
				if (!black[it->second._index]) {
					x |= dfs_search_cycles(&it->second, red, black);
				}
					
			}
			return x;
		}

		std::vector<std::vector<Node<T>>> tarjan() {
			// Counter for the index
			int ticks = 0;

			// Create the stack call
			std::stack<Node<T>*> S;

			// Create the index array in order to store the indexes of each node 
			int index[_map.size()];
			init_struct<int>(index, _map.size(), -1); // init to -1 = Not visited yet

			// Create the lowlink index array
			int lowlink[_map.size()];
			init_struct<int>(lowlink, _map.size(), -1); // init to -1;

			// Create stack cache to know if a node is in a stack
			bool stack[_map.size()];
			init_struct<bool>(stack, _map.size(), false);

			// Vector with the components
			std::vector<std::vector<Node<T>>> components;

			typename std::unordered_map<T, Node<T>>::iterator it; 
			for (it = _map.begin(); it != _map.end(); ++it) {
				// If node has not been visited yet
				if (index[it->second._index] == -1) {
					strongconnect(&it->second, ticks, S, index, lowlink, stack, components); // Recursive function to find elements strog-connected from this node
				}
					
			}
			return components;
		}

		//metadata
		int n_nodes = 0;
		int n_edges = 0;


	private:

		// Init array
		template <typename A>
		void init_struct(A *index, int size, A init) {
			for (int i = 0; i < size; i++) {
				index[i] = init;
			}
		}

		/**
		 * 
		 * Search for cycles
		 *
		 */
		bool dfs_search_cycles(Node<T> *v, bool red[], bool black[]) {
			// Node already visited in other path, no need to check again
			if (black[v->_index]) return false; 
			// Node already visited in the same path -> Cycle encountered 
			if (red[v->_index]) return true;
			red[v->_index] = true;
			bool x = false;
			for (int i = 0; i < v->_edges.size(); i++) { 
				Node<T> *w = v->_edges.at(i); //For each edge of node v -> sort topological
				x |= dfs_search_cycles(w, red, black);
				if (x) return x;
			}
			black[v->_index] = true; //mark as seen
			return x;
		}

		/**
		 * 
		 * Topological sort
		 * Linear sort of all the elements keeping the initial union between vertices
		 * 
		 * @black[]: array of elements that have been hard-visited, that means that have been visited at least once in any path
		 * @red[]: array of elements visited in this path 
		 *
		 * 
		 */
		void dfs_topological_sort(Node<T> *v, std::stack<Node<T>> &list, bool red[], bool black[]) {
			// Node already visited in other path, no need to check again
			if (black[v->_index]) return; 
			// Node already visited in the same path -> Cycle encountered 
			if (red[v->_index]) return;
			red[v->_index] = true;
			for (int i = 0; i < v->_edges.size(); i++) { 
				Node<T> *w = v->_edges.at(i); //For each edge of node v -> sort topological
				dfs_topological_sort(w, list, red, black);
			}
			black[v->_index] = true; //mark as seen
			list.push(*v);
			return; //return true -> Acyclid
		}


		/**
		 *  @index = number to identify the node and check for the start of a strong component
		 * 	@lowlink = smalles @index reachable from that node
		 * 	@stack = keeps track of the nodes already visited from the first random node in order to keep make sure
		 * 			 that the lowlink is updated on the return only if it has been visited before in this call
		 */

		void strongconnect(Node<T> *v, int &ticks, std::stack<Node<T>*> &S, int index[], int lowlink[], bool stack[], std::vector<std::vector<Node<T>>> &components) {
			
			int node = v->_index;

			// Assing each node a unique number when visited
			index[node] = ticks;

			// Assing each node the same index to the lowlink -> Only node that it can reach from here for now is himself
			lowlink[node] = ticks;

			// Increment counter to ensure unique indexes
			ticks += 1;

			S.push(v); // O(1)
			stack[node] = true;

			for (int i = 0; i < v->_edges.size(); ++i) {
				Node<T> *w = v->_edges.at(i);
				if (index[w->_index] == -1) { // If node has not been visited
					strongconnect(w, ticks, S, index, lowlink, stack, components);
				}
				if (stack[w->_index]) { // If node is in the stack aka it has been visited in the same path
					lowlink[node] = std::min(lowlink[node], lowlink[w->_index]); // update the lowlink with the w if is smaller aka there exist a path from v to w 
				}
			}

			
			
			// Add the strong connected component to the list only if is the start of a cycle or is himself
			if (lowlink[node] == index[node]) {
				Node<T> *w;
				std::vector<Node<T>> component;
				do {
					w  = S.top();
					S.pop(); // Pop each node of the cycle from the stack to make sure is only reachable from this path
					stack[w->_index] = false;
					component.push_back(*w); // O(1) -> Amortized time
				} while (w->_index != v->_index);
				components.push_back(component);
			}
			return;	
		}
		

        std::unordered_map<T, Node<T>> _map;

		
};