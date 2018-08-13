#include "wdgraph.hpp"

namespace lgraph {
namespace utils {

// PROTECTED

template<class T>
void wdgraph<T>::get_unique_edges(set<pair<edge, T> >& unique_edges) const {
	for (node i = 0; i < this->n_nodes(); ++i) {
		const neighbourhood& ni = this->adjacency_list[i];
		const weight_list<T>& wi = this->weights[i];

		for (size_t it = 0; it < ni.n_elems(); ++it) {

			// this graph is DIRECTED so an edge is the
			// pair of nodes (i,ni[it]) interpreted as
			// "i points to ni[it]"
			pair<edge, T> e((i, ni[i]), wi[i]);

			bool new_edge = unique_edges.find(e) == unique_edges.end();
			if (new_edge) {
				unique_edges.insert(e);
			}

		}
	}
}

// PUBLIC

template<class T>
wdgraph<T>::wdgraph() : wxgraph<T>() {
}

template<class T>
wdgraph<T>::~wdgraph() { }

// MODIFIERS

template<class T>
void wdgraph<T>::add_edge(const edge& e, const T& w) {
	add_edge(e.first, e.second, w);
}

template<class T>
void wdgraph<T>::add_edge(node u, node v, const T& w) {
	assert( this->has_node(u) );
	assert( this->has_node(v) );
	assert( not this->has_edge(u,v) );

	this->adjacency_list[u].add(v);
	this->weights[u].add(w);

	this->num_edges += 1;
}

template<class T>
void wdgraph<T>::remove_edge(const edge& e) {
	remove_edge(e.first, e.second);
}

template<class T>
void wdgraph<T>::remove_edge(node u, node v) {
	assert( this->has_node(u) );
	assert( this->has_node(v) );
	assert( this->has_edge(u,v) );

	bool erased = false;

	neighbourhood& nu = this->adjacency_list[u];
	weight_list<T>& wu = this->weights[u];

	// find the position of node v in neighbourhood of u
	// delete the neighbour and the corresponding weight
	size_t posu = this->get_neighbour_position(nu, v);
	if (posu < nu.n_elems()) {
		nu.remove(posu);
		wu.remove(posu);
		erased = true;
	}

	// decrease number of edges only if necessary
	if (erased) {
		this->num_edges -= 1;
	}
}

} // -- namespace utils
} // -- namespace lgraph