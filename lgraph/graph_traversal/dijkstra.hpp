/*********************************************************************
 * lgraph
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#pragma once

// C++ includes
#include <functional>

// lgraph includes
#include <lgraph/data_structures/wxgraph.hpp>

namespace lgraph {
namespace traversal {
namespace dijkstra {

/// A pair encoding the distance to a node, and that node.
template<class T>
using djka_node = std::pair<T, node>;

/**
 * @brief Terminating function.
 *
 * Returns true if the @ref Dijkstra algorithm should terminate.
 *
 * For more details on when this function is called see @ref Dijkstra.
 *
 * @param G The graph being traversed.
 * @param u The node at the front of the queue of the algorithm and the distance
 *	to it.
 * @param vis The set of visited nodes.
 */
template<class T = size_t>
using djka_terminate =
std::function<
	bool (const wxgraph<T> *G,const djka_node<T>& u, const std::vector<bool>& vis)
>;

/**
 * @brief Node processing function.
 *
 * Processes the current node visited.
 *
 * For more details on when this function is called see @ref Dijkstra.
 *
 * @param G The graph being traversed.
 * @param u The node at the front of the queue of the algorithm and the distance
 *	to it.
 * @param vis The set of visited nodes.
 */
template<class T = size_t>
using djka_process_current =
std::function<
	void (const wxgraph<T> *G, const djka_node<T>& u, const std::vector<bool>& vis)
>;

/**
 * @brief Node processing function.
 *
 * Processes the next visited node.
 *
 * For more details on when this function is called see @ref Dijkstra.
 *
 * @param G The graph being traversed.
 * @param u The node at the front of the queue of the algorithm.
 * @param v The node neighbour of @e u visited by the algorithm.
 * @param w weight of edge (u,v).
 * @param vis The set of visited nodes.
 *
 * @return Returns true or false whether the next pair of (distance,node)
 *	should be added or not.
 */
template<class T = size_t>
using djka_process_neighbour =
std::function<
	bool (const wxgraph<T> *G, node u, node v, const T& w, const std::vector<bool>& vis)
>;

/**
 * @brief Generic Dijkstra algorithm.
 *
 * The procedure applied for this algorithm here
 * for traversing through a graph from a source node to
 * a target node is as follows:
 *
 * <pre>
 * Dijkstra(graph, source, target):
 *	 1.	vis = {false}	// set of |V(graph)| bits set to false
 *	 2.	Q = empty		// empty priority queue of (distance,node)
 *	 3.	while Q is not empty do
 *	 4.		(wu,u) = Q.front
 *	 5.		remove Q's front
 *   6.		if u is not visited then
 *	 7.			proc_curr(G,u,vis)
 *	 8			if terminate(G,u,vis) then terminate
 *	 9.			else
 *	10.				Nu = neighbourhood of u
 *	11.				for each v in Nu do
 *	12.					wv := weight edge (u,v)
 *	13.					add_next := proc_neigh(G, u,v, wv, vis)
 *	14.					if add_next then
 *	15.						push w into Q
 *	16.					endif
 *	17.				endfor
 *	18.			endif
 *	19.		endif
 *	20.	endwhile
 * </pre>
 *
 * @param G The graph being traversed.
 * @param source The node where the algorithm starts at.
 * @param term The terminating function. It is used as a
 * termination condition in line 8.
 * @param proc_curr The function to process the currently visited node.
 * It is called in line 7 used to perform some operation on the current
 * node of the traversal.
 * @param proc_neigh It is called between the 'for' loop in line 13 and
 * the used to perform some operation on each of the neighbours.
 */
template<class T>
void Dijkstra
(
	const wxgraph<T> *G,
	node source,
	djka_terminate<T> term =
		[](const wxgraph<T> *, const djka_node<T>&, const std::vector<bool>&)
		-> bool { return false; },
	djka_process_current<T> proc_curr =
		[](const wxgraph<T> *, const djka_node<T>&, const std::vector<bool>&)
		-> void {},
	djka_process_neighbour<T> proc_neigh =
		[](const wxgraph<T> *, node, node, const T&, const std::vector<bool>&)
		-> bool { return true; }
);

} // -- namespace dijkstra
} // -- namespace traversal
} // -- namespace lgraph

#include <lgraph/graph_traversal/dijkstra.cpp>
