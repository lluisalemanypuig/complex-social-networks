#include "traversal.hpp"

namespace dsa {
namespace traversal {

	/// NODE-NODE

	template<class T>
	void xwpath(const xxgraph<T> *G, node source, node target, node_path<T>& p) {
		assert(G->has_node(source));
		assert(G->has_node(target));

		const size_t N = G->n_nodes();

		// prev[v] = u: previous node of v in the path is u
		vector<node> prev(N, N + 1);
		vector<T> ds(N, utils::inf_t<T>());
		ds[source] = 0;

		// terminate when target is found
		djka_terminate<T> terminate =
		[&target](const xxgraph<T> *, const djka_dist_node<T>& u, const vector<bool>&) -> bool
		{
			return u.second == target;
		};

		djka_process_current<T> proc_curr =
		[] (const xxgraph<T> *, const djka_dist_node<T>&, const vector<bool>&) -> void { };

		// function to compute the shortest distance from node u to node v
		djka_process_neighbour<T> proc_neig =
		[&prev, &ds]
		(const xxgraph<T> *, node u, node v, const T& w, const vector<bool>&) -> bool
		{
			bool add = false;
			if (ds[u] + w < ds[v]) {
				ds[v] = ds[u] + w;
				prev[v] = u;
				add = true;
			}
			return add;
		};

		Dijkstra(G, source, terminate, proc_curr, proc_neig);

		p.empty();
		node l = target;
		while (l != source and prev[l] != N + 1) {
			p.add_node(l);
			l = prev[l];
		}
		if (l == N + 1) {
			p.empty();
		}
		else {
			p.add_node(source);
			p.reverse();
		}
		p.set_length(ds[target]);
	}

	template<class T>
	void xwpath(const xxgraph<T> *G, node source, node target, node_path_set<T>& ps) {
		// all paths from source to the other nodes
		vector<node_path_set<T> > node_all_paths;
		xwpath(G, source, node_all_paths);
		ps = node_all_paths[target];
	}

	/// NODE-ALL

	template<class T>
	void xwpath(const xxgraph<T> *G, node source, vector<node_path<T> >& ps) {
		assert(G->has_node(source));

		const size_t N = G->n_nodes();

		// prev[v] = u: previous node of v in the path is u
		vector<node> prev(N, N + 1);
		vector<T> ds(N, utils::inf_t<T>());
		ds[source] = 0;

		// terminate when target is found
		djka_terminate<T> terminate =
		[](const xxgraph<T> *, const djka_dist_node<T>&, const vector<bool>&) -> bool
		{ return false; };

		djka_process_current<T> proc_curr =
		[] (const xxgraph<T> *, const djka_dist_node<T>&, const vector<bool>&) -> void { };

		// function to compute the shortest distance from node u to node v
		djka_process_neighbour<T> proc_neig =
		[&prev, &ds]
		(const xxgraph<T> *, node u, node v, const T& w, const vector<bool>&) -> bool
		{
			bool add = false;
			if (ds[u] + w < ds[v]) {
				ds[v] = ds[u] + w;
				prev[v] = u;
				add = true;
			}
			return add;
		};

		Dijkstra(G, source, terminate, proc_curr, proc_neig);

		ps = vector<node_path<T> >(N);
		for (node u = 0; u < N; ++u) {
			node_path<T>& p = ps[u];

			p.empty();
			node l = u;
			while (l != source and l != N + 1) {
				p.add_node(l);
				l = prev[l];
			}
			if (l == N + 1) {
				p.empty();
			}
			else {
				p.add_node(source);
				p.reverse();
			}
			p.set_length(ds[u]);
		}
	}

	template<class T>
	void xwpath(const xxgraph<T> *G, node source, vector<node_path_set<T> >& ps) {

		logger<cout_stream>& LOG = logger<cout_stream>::get_logger();
		LOG.log() << "template<class T> void xwpath(const xxgraph<T> *G, node source, vector<node_path_set<T> >& ps)" << endl;
		LOG.log() << "    no implementation of one-to-all shortest path" << endl;

		ps = vector<node_path_set<T> >(G->n_nodes());
	}

	/// ALL-ALL

	template<class T>
	void xwpaths(const xxgraph<T> *G, vector<vector<node_path<T> > >& all_all_paths) {
		const size_t N = G->n_nodes();

		// allocate memory...
		vector<vector<T> > dist(N, vector<T>(N, utils::inf_t<T>()));
		all_all_paths = vector<node_path_set<T> >(N, node_path_set<T>(N));

		// initialise data
		for (size_t u = 0; u < N; ++u) {

			// all paths starting at a node with degree > 0 start with that node
			for (size_t v = 0; v < N; ++v) {
				if (G->degree(u) > 0 or u == v) {
					all_all_paths[u][v].add_node(u);
				}
			}

			// initialise paths for each pair of neighbouring nodes
			const neighbourhood& Nu = G->get_neighbours(u);
			vector<T> weights;
			G->get_weights(u, weights);

			for (size_t v_it = 0; v_it < Nu.size(); ++v_it) {
				size_t v = Nu[v_it];

				dist[u][v] = weights[v_it];
				all_all_paths[u][v].add_node(v);
				all_all_paths[u][v].add_length(weights[v_it]);
			}
		}

		// find the all-to-all shortest paths (N^3)
		for (size_t w = 0; w < N; ++w) {
			// distance from a vertex to itself is 0
			dist[w][w] = 0;

			for (size_t u = 0; u < N; ++u) {
				for (size_t v = 0; v < N; ++v) {

					// ignore the cases where:
					// the path is not moving
					if (u == v) continue;
					// the distances are infinite
					if (dist[v][w] == utils::inf_t<_new_>()) continue;
					if (dist[w][u] == utils::inf_t<_new_>()) continue;

					T d = dist[u][w] + dist[w][v];
					if (d < dist[u][v]) {
						dist[u][v] = d;

						all_all_paths[u][v] = all_all_paths[u][w];
						all_all_paths[u][v].concatenate(all_all_paths[w][v]);
					}
				}
			}
		}
	}

	template<class T>
	void xwpaths(const xxgraph<T> *G, vector<vector<node_path_set<T> > >& all_all_paths) {
		const size_t N = G->n_nodes();

		// allocate memory...
		vector<vector<T> > dist(N, vector<T>(N, utils::inf_t<T>()));
		all_all_paths = vector<vector<node_path_set<T> > >(N, vector<node_path_set<T> >(N));

		// initialise with edge weights (here always 1) the distance and the
		// shortest-path from u to all its neighbours with {u,v}
		for (size_t u = 0; u < N; ++u) {
			const neighbourhood& Nu = G->get_neighbours(u);
			vector<T> weights;
			G->get_weights(u, weights);

			for (size_t v_it = 0; v_it < Nu.size(); ++v_it) {
				size_t v = Nu[v_it];

				dist[u][v] = weights[v_it];
				all_all_paths[u][v] = node_path_set<T>(1);
				all_all_paths[u][v][0].add_node(u);
				all_all_paths[u][v][0].add_node(v);
				all_all_paths[u][v][0].add_length(weights[v_it]);
			}
		}

		// find all the all-to-all shortest paths (N^3)
		for (size_t w = 0; w < N; ++w) {
			// distance from a vertex to itself is 0, the path just {w}
			dist[w][w] = 0;
			all_all_paths[w][w] = node_path_set<T>(1, node_path<T>(w));

			for (size_t u = 0; u < N; ++u) {
				for (size_t v = 0; v < N; ++v) {

					// ignore the cases where:
					// the path is not moving
					if (u == v) continue;
					// the distances are infinite
					if (dist[v][w] == utils::inf_t<_new_>()) continue;
					if (dist[w][u] == utils::inf_t<_new_>()) continue;

					T d = dist[u][w] + dist[w][v];
					if (d < dist[u][v]) {
						// this is a shorter path than the shortest found so far
						dist[u][v] = d;

						if (u != w and w != v) {
							// concatenate each path {u, ..., w}
							// with each path {w, ..., v} (without repeating the w)

							size_t n_uw = all_all_paths[u][w].size();
							size_t n_wv = all_all_paths[w][v].size();
							all_all_paths[u][v] = node_path_set<T>( n_uw*n_wv );

							size_t uv = 0;
							for (size_t uw = 0; uw < n_uw; ++uw) {
								for (size_t wv = 0; wv < n_wv; ++wv) {
									all_all_paths[u][v][uv] = all_all_paths[u][w][uw];
									all_all_paths[u][v][uv].concatenate( all_all_paths[w][v][wv] );
									++uv;
								}
							}
						}
					}
					else if (d == dist[u][v]) {
						// this is a path as short as the shortest found so far
						if (u != w and w != v) {

							// if the path found is as long as the shortest,
							// just add the new ones: cartesian product of
							// the current paths:
							// append to all_all_paths[u][v]
							// all_all_paths[u][w] x all_all_paths[w][v]

							size_t n_wv = all_all_paths[w][v].size();

							for (size_t wv = 0; wv < n_wv; ++wv) {
								// place iterator at the position corresponding
								// to the first added path from all_all_paths[u][w]
								size_t uv = all_all_paths[u][v].size();

								all_all_paths[u][v].insert(
									all_all_paths[u][v].end(),
									all_all_paths[u][w].begin(), all_all_paths[u][w].end()
								);

								// append each path from 'w' to 'v' to the new added paths
								for (size_t s = uv; s < all_all_paths[u][v].size(); ++s) {
									all_all_paths[u][v][s].concatenate( all_all_paths[w][v][wv] );
								}
							}
						}
					}
				}
			}
		}
	}

} // -- namespace traversal
} // -- namespace dsa
