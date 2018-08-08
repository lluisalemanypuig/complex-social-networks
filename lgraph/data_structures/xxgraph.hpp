#pragma once

// C++ includes
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

// Custom includes
#include <lgraph/utils/definitions.hpp>

namespace lgraph {
namespace utils {

/**
 * @brief Abstract class for the graph data structure.
 *
 * This interface requires the implementation of several methods
 * to complete the implementation of a data structure for graphs
 * that uses adjacency lists. It has only two attributes: @ref adjacency_list,
 * that stores the adjacency list of each node, and @ref num_edges
 * that contains the number of edges in the graph. Depending on
 * the type of graph implemented these two attributes should be
 * modified accordingly.
 *
 * @param T In case of weighted graphs, this parameter indicates
 * the type of the edge weights
 */
template<class T>
class xxgraph {
	protected:
		/// The neighbourhood of every node
		vector<neighbourhood> adjacency_list;
		/// The amount of edges in this graph
		size_t num_edges;

		/**
		 * @brief Returns a constant iterator to node @e u's position in the neighbourhood @e n
		 *
		 * If the iterator returned is not at the end of the list then @e u is in the list.
		 * Performs a linear search to find it.
		 * @param n The neighbourhood of a node in the graph
		 * @param u The node to look for in the neighbourhood
		 * @return Returns a constant iterator to @e n.end() if @e u is not in the list.
		 * Returns a constant iterator to the position of @e u if otherwise.
		 */
		ncit cget_neighbour_position(const neighbourhood& n, node u) const;

		/**
		 * @brief Returns a non-constant iterator to node @e u's position in the neighbourhood @e n
		 *
		 * If the iterator returned is not at the end of the list then @e u is in the list.
		 * Performs a linear search to find it.
		 * @param n The neighbourhood of a node in the graph
		 * @param u The node to look for in the neighbourhood
		 * @return Returns a non-constant iterator to @e n.end() if @e u is not in the list.
		 * Returns a non-constant iterator to the position of @e u if otherwise.
		 */
		nit get_neighbour_position(neighbourhood& n, node u);

	protected:

		/// Initialise the list of neighbourhoods with @e n instances
		virtual void initialise_adjacency_list(size_t n) = 0;

		/**
		 * @brief Initialise the list of list of weights
		 *
		 * A subclass representing an unweighted graph may leave the body empty.
		 */
		virtual void initialise_weights(size_t n) = 0;

		/// Clear the list of neighbourhoods
		virtual void clear_adjacency_list() = 0;

		/**
		 * @brief Clear the list of weights
		 *
		 * A subclass representing an unweighted graph may leave the body empty.
		 */
		virtual void clear_weights() = 0;

	public:
		/// Constructor
		xxgraph();
		/// Destructor
		virtual ~xxgraph();

		/**
		 * @brief Initialises the attributes of a graph with @e n nodes
		 *
		 * First, it clears all the memory allocated so far. Then, initialises all
		 * the attributes so that it can store all the necessary information.
		 * @param n Number of nodes of the graph
		 */
		void init(size_t n);

		/**
		 * @brief Adds one node to the graph
		 *
		 * @return Returns the index of the new node
		 */
		size_t add_node();

		/**
		 * @brief Adds @e n nodes to the graph
		 *
		 * The nodes are assigned consecutive, increasing values.
		 * @return Returns the index of the last node.
		 */
		size_t add_n_nodes(node n);

		/**
		 * @brief Adds an edge to this graph.
		 *
		 * The attribute @ref num_edges is incremented by one.
		 * @param e A pair of nodes
		 * @param w The weight of the edge
		 */
		virtual void add_edge(const edge& e, T w = 0) = 0;

		/**
		 * @brief Adds all edges taken from a list
		 *
		 * The attribute @ref num_edges is incremented as many times
		 * as elements there are in @e edge_list.
		 * @param edge_list A list of pairs of nodes
		 * @param ws A list of weights. The i-th edge has weight @e ws[i].
		 */
		virtual void add_edges(const vector<edge>& edge_list, const vector<T>& ws = vector<T>()) = 0;

		/**
		 * @brief Adds an edge between nodes @e u and @e v
		 *
		 * The attribute @ref num_edges is incremented by one.
		 *
		 * @param u The fist node of the edge
		 * @param v The second node of the edge
		 * @param w The weight of the edge
		 */
		virtual void add_edge(node u, node v, T w = 0) = 0;

		/**
		 * @brief Removes an edge from this graph.
		 *
		 * The attribute @ref num_edges is decremented by one.
		 * @param e A pair of nodes
		 */
		virtual void remove_edge(const edge& e) = 0;

		/**
		 * @brief Removes all edges taken from a list
		 *
		 * The attribute @ref num_edges is decremented by one.
		 * @param edge_list A list of edges
		 */
		virtual void remove_edges(const vector<edge>& edge_list) = 0;

		/**
		 * @brief Removes an edge from this graph.
		 *
		 * The attribute @ref num_edges is decremented by one.
		 * @param u The fist node of the edge
		 * @param v The second node of the edge
		 */
		virtual void remove_edge(node u, node v) = 0;

		/**
		 * @brief Deletes all memory used by the graph
		 *
		 * The value @ref num_edges is set to 0.
		 */
		void clear();

		/// Returns true if node @e u is in this graph.
		bool has_node(node u) const;

		/**
		 * @brief Returns true if there is an edge between nodes @e u and @e v
		 * @pre @e u and @e v must be in the graph
		 */
		virtual bool has_edge(node u, node v) const = 0;

		/**
		 * @brief Returns the number of nodes
		 * @return Returns the size of @ref adjacency_list
		 */
		size_t n_nodes() const;

		/**
		 * @brief Returns the number of edges
		 * @return Returns the value @ref num_edges
		 */
		size_t n_edges() const;

		/// Returns all nodes (as integers)
		void nodes(vector<node>& all_nodes) const;

		/**
		 * @brief Returns the neighbourhood of node u
		 * @param u The node whose neighbourhood we want
		 * @pre @e u must be a node from the graph
		 */
		const neighbourhood& get_neighbours(node u) const;

		/**
		 * @brief Returns the number of neighbours of u
		 * @param u The node whose neighbourhood size we want
		 * @return Returns the size of the neighbourhood of @e u,
		 * that is, the size of the list in @ref adjacency_list[u]
		 * @pre @e u must be a node from the graph
		 */
		size_t degree(node u) const;

		/**
		 * @brief Returns the weight of the edge (@e u, @e v)
		 * @pre The edge (@e u, @e v) must be in the graph
		 */
		virtual T edge_weight(node u, node v) const = 0;

		/**
		 * @brief Returns the weights to all neighbours of node @e u
		 * @pre @e u must be in the graph
		 */
		virtual void get_weights(node u, vector<T>& ws) const = 0;

		/// Reads the graph from a file
		virtual bool read_from_file(const string& filename) = 0;
		/// Reads the graph from a file
		virtual bool read_from_file(const char *filename) = 0;

		/// Stores the graph in a file
		virtual bool store_in_file(const string& filename) = 0;
		/// Stores the graph in a file
		virtual bool store_in_file(const char *filename) = 0;

		/// Returns the adjacency matrix of this graph
		void get_adjacency_matrix(vector<vector<bool> >& adj_mat) const;

		/**
		 * @brief Returns the degree sequence of the graph
		 * @param[out] ds A list of pairs: for each degree the amount
		 * of nodes in this graph that have that degree. The degree
		 * of a node is detailed in @ref degree
		 */
		void get_degree_sequence(map<size_t, size_t>& ds) const;

		/**
		 * @brief Returns the number of triangles in this graph
		 * @return Returns the number of cycles of length 3
		 */
		size_t n_triangles() const;
};

} // -- namespace utils
} // -- namespace lgraph

#include "xxgraph.cpp"