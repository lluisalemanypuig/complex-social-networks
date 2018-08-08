#pragma once

// C++ incldues
#include <algorithm>
#include <limits>
#include <vector>
using namespace std;

// Custom includes
#include <lgraph/data_structures/boolean_path.hpp>
#include <lgraph/data_structures/uugraph.hpp>
#include <lgraph/graph_traversal/traversal.hpp>

namespace lgraph {
using namespace utils;

namespace networks {
namespace metrics {
namespace centralities {
	
	/* DEGREE CENTRALITY */

	/**
	 * @brief Normalised degree centrality of a single node
	 * @param G The graph to be evaluated
	 * @param u The node to be evaluated
	 * @return Returns the degree of node @e u divided by the
	 * number of nodes (minus 1)
	 */
	double degree(const uugraph& G, node u);
	/**
	 * @brief Normalised degree centrality of all nodes
	 * @param[in] G The graph to be evaluated
	 * @param[out] The i-th position contains the normalised degree
	 * centrality of the i-th node.
	 */
	void degree(const uugraph& G, vector<double>& dc);
	
	/*  CLOSENESS CENTRALITY */

	/**
	 * @brief Closeness centrality of a node
	 * @param G The graph to be evaluated
	 * @param u The node to be evaluated
	 * @return Returns the closeness centrality of a node.
	 */
	double closeness(const uugraph& G, node u);
	/**
	 * @brief Closeness centrality of all nodes of a graph
	 *
	 * Computes the distance between each pair of nodes in the graph.
	 * @param[in] G The graph to be evaluated
	 * @param[out] cc The @e i-th position contains the
	 * closeness centrality of the @e i-th node.
	 */
	void closeness(const uugraph& G, vector<double>& cc);
	/**
	 * @brief Closeness centrality of all nodes of a graph
	 *
	 * Uses the parameter @e atad to avoit computing the all-to-all distances
	 * (the distance between each pair of nodes).
	 * @param[in] G The graph to be evaluated
	 * @param[in] atad The matrix with the distance between each pair of nodes.
	 * @param[out] cc The @e i-th position contains the
	 * closeness centrality of the @e i-th node.
	 */
	void closeness(const uugraph& G, const vector<vector<_new_> >& atad, vector<double>& cc);
	
	/* BETWEENNESS CENTRALITY */

	/**
	 * @brief Betweenness centrality of a node
	 *
	 * This centrality measure is implemented following the recommendations
	 * given in [Newman, 2010]:
	 *	- Any path from vertex 's' to vertex 't' is considered to include both
	 *		endpoints.
	 *	- The equation to calculate the centrality also considers paths from any
	 *		vertex to itself.
	 *
	 * Furthermore, only the pairs of vertices (s,t) with s <= t are considered.
	 * Finally, this metric is also normalised.
	 *
	 * [Newman, 2010] Newman, M. E. J. (2010). Networks. An introduction. Oxford
	 * University Press, Oxford.
	 *
	 * This function computes all shortest paths between each pair of nodes.
	 * @param G The graph to be evaluated
	 * @param u The node to be evaluated
	 * @return Returns the betweenness centrality of a node, considering
	 * that a path between @e u and @e v contains both @u and @e v.
	 */
	double betweenness(const uugraph& G, node u);
	/**
	 * @brief Betweenness centrality of a node
	 *
	 * This centrality measure is implemented following the recommendations given
	 * in [Newman, 2010]:
	 *	- Any path from vertex 's' to vertex 't' is considered to include both
	 *		endpoints.
	 *	- The equation to calculate the centrality also considers paths from any
	 *		vertex to itself.
	 *
	 * Furthermore, only the pairs of vertices (s,t) with s <= t are considered.
	 * Finally, this metric is also normalised.
	 *
	 * [Newman, 2010] Newman, M. E. J. (2010). Networks. An introduction. Oxford
	 * University Press, Oxford.
	 *
	 * This uses the parameter @e paths to avoid computing all the shortest paths between
	 * all pair of nodes.
	 * @param G The graph to be evaluated
	 * @param paths @e paths[u][v] contains all shortest paths between @e u and @e v
	 * for 0 <= u,v < @e G.n_nodes()
	 * @param u The node to be evaluated
	 * @return Returns the betweenness centrality of a node, considering
	 * that a path between @e u and @e v contains both @u and @e v.
	 */
	double betweenness(const uugraph& G, const vector<vector<boolean_path_set<_new_> > >& paths, node u);
	/**
	 * @brief Betweenness centrality of all nodes in a graph
	 *
	 * This centrality measure is implemented following the recommendations given
	 * in [Newman, 2010]:
	 *	- Any path from vertex 's' to vertex 't' is considered to include both
	 *		endpoints.
	 *	- The equation to calculate the centrality also considers paths from any
	 *		vertex to itself.
	 *
	 * Furthermore, only the pairs of vertices (s,t) with s <= t are considered.
	 * Finally, this metric is also normalised.
	 *
	 * [Newman, 2010] Newman, M. E. J. (2010). Networks. An introduction. Oxford
	 * University Press, Oxford.
	 *
	 * This function computes all shortest paths between each pair of nodes.
	 * @param[in] G The graph to be evaluated
	 * @param[out] bc @ref bc[i] contains the betweenness centrality of the @e i-th node
	 */
	void betweenness(const uugraph& G, vector<double>& bc);
	/**
	 * @brief Betweenness centrality of a node
	 *
	 * This centrality measure is implemented following the recommendations given
	 * in [Newman, 2010]:
	 *	- Any path from vertex 's' to vertex 't' is considered to include both
	 *		endpoints.
	 *	- The equation to calculate the centrality also considers paths from any
	 *		vertex to itself.
	 *
	 * Furthermore, only the pairs of vertices (s,t) with s <= t are considered.
	 * Finally, this metric is also normalised.
	 *
	 * [Newman, 2010] Newman, M. E. J. (2010). Networks. An introduction. Oxford
	 * University Press, Oxford.
	 *
	 * This uses the parameter @e paths to avoid computing all the shortest paths between
	 * all pair of nodes.
	 * @param[in] G The graph to be evaluated
	 * @param[in] paths @e paths[u][v] contains all shortest paths between @e u and @e v
	 * for 0 <= u,v < @e G.n_nodes()
	 * @param[out] bc @ref bc[i] contains the betweenness centrality of the @e i-th node
	 */
	void betweenness(const uugraph& G, const vector<vector<boolean_path_set<_new_> > >& paths, vector<double>& bc);
	
} // -- namespace centralities
} // -- namespace metrics
} // -- namespace networks	
} // -- namespace lgraph
