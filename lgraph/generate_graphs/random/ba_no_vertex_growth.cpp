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

#include <lgraph/generate_graphs/random/barabasi_albert.hpp>

// lgraph includes
#include <lgraph/utils/logger.hpp>

namespace lgraph {
namespace networks {
namespace random {

// private namespace for the no growth variant
namespace _ng {

	// function for debugging
	inline void print_stubs
	(
		const uugraph& G, const std::vector<size_t>& stubs,
		size_t max_idx, const std::string& tab = ""
	)
	{
		utils::logger<utils::null_stream>& LOG =
			utils::logger<utils::null_stream>::get_logger();

		LOG.log() << tab << "max_idx= " << max_idx << std::endl;
		LOG.log() << tab << "indexes: ";
		for (size_t i = 0; i < stubs.size(); ++i) {
			size_t pos_length = std::to_string(i).length();
			size_t stub_length = std::to_string(stubs[i]).length();
			size_t degree_length = std::to_string(G.degree(stubs[i])).length();
			size_t max_length = std::max(pos_length, std::max(stub_length, degree_length));

			LOG.log() << std::setw(max_length) << i << " ";
		}
		LOG.log() << std::endl;

		LOG.log() << tab << "stubs  : ";
		for (size_t i = 0; i < stubs.size(); ++i) {
			size_t pos_length = std::to_string(i).length();
			size_t stub_length = std::to_string(stubs[i]).length();
			size_t degree_length = std::to_string(G.degree(stubs[i])).length();
			size_t max_length = std::max(pos_length, std::max(stub_length, degree_length));

			LOG.log() << std::setw(max_length) << stubs[i] << " ";
		}
		LOG.log() << std::endl;

		LOG.log() << tab << "degree : ";
		for (size_t i = 0; i < stubs.size(); ++i) {
			size_t pos_length = std::to_string(i).length();
			size_t stub_length = std::to_string(stubs[i]).length();
			size_t degree_length = std::to_string(G.degree(stubs[i])).length();
			size_t max_length = std::max(pos_length, std::max(stub_length, degree_length));

			LOG.log() << std::setw(max_length) << G.degree(stubs[i]) << " ";
		}
		LOG.log() << std::endl;

		LOG.log() << tab << "         ";
		for (size_t i = 0; i < max_idx; ++i) {
			size_t pos_length = std::to_string(i).length();
			size_t stub_length = std::to_string(stubs[i]).length();
			size_t max_length = std::max(pos_length, stub_length);

			LOG.log() << std::setw(max_length) << " " << " ";
		}
		size_t pos_length = std::to_string(max_idx).length();
		size_t stub_length = std::to_string(stubs[max_idx]).length();
		size_t max_length = std::max(pos_length, stub_length);
		LOG.log() << std::setw(max_length) << "^" << std::endl;
	}

	// G: the graph
	// u: the source vertex
	// stub_idx: the index of the vertex chosen
	// stubs: a reference to the vector of stubs
	// max_idx: upper bound of the range of the stubs vector
	inline void update_stubs
	(const uugraph& G, size_t stub_idx, std::vector<size_t>& stubs, size_t& max_idx)
	{
		const size_t v = stubs[stub_idx];

		if (G.degree(v) == 0) {
			// there is one stub for vertex 'v' but degree is 0. This
			// is a special case with which has to be dealt with care

			if (stub_idx == max_idx) {
				// easy case: move max_idx
				--max_idx;

				return;
			}

			size_t next_pos = stub_idx + 1;
			std::copy(
				stubs.begin() + next_pos, stubs.begin() + max_idx + 1,
				stubs.begin() + stub_idx
			);
			stubs[max_idx] = v;
			--max_idx;

			return;
		}

		// the chosen node goes at the end, where it will not be chosen again.

		if (v == stubs[max_idx]) {
			// connect the new vertex with a vertex pointed by max_idx
			// swap the regions corresponding to the stubs of v and
			// what comes after max_idx. Update max_idx

			std::copy(
				stubs.begin() + max_idx + 1, stubs.end(),
				stubs.begin() + max_idx - G.degree(v) + 1
			);

			std::fill(stubs.end() - G.degree(v), stubs.end(), v);

			stubs.push_back(v);
			max_idx = max_idx - G.degree(v);
			return;
		}

		// most dificult case: it is time to copy memory.. carefully!

		size_t next_pos = stub_idx;
		while (next_pos < max_idx and stubs[next_pos] == v) {
			++next_pos;
		}

		size_t last_pos = next_pos - G.degree(v);

		std::copy(
			stubs.begin() + next_pos, stubs.end(),
			stubs.begin() + last_pos
		);

		size_t q = stubs.size() - max_idx - 1;
		max_idx = max_idx - G.degree(v);
		size_t lim_inf = max_idx + q + 1;

		std::fill(stubs.begin() + lim_inf, stubs.end(), v);
		stubs.push_back(v);
	}

	// Builds the stubs needed for the random choice of vertices.
	// Returns the maximum amount of new neighbours we can connect to u
	inline size_t update_stubs_initial
	(
		const uugraph& G, size_t u, size_t m0,
		std::vector<size_t>& stubs, size_t& max_idx
	)
	{
		// put vertices that are not neighbours of u at the beginning

		size_t stubs_so_far = 0;
		for (size_t v = 0; v < G.n_nodes(); ++v) {
			if (v != u and not G.has_edge(u, v)) {
				size_t stubs_v = G.degree(v);
				if (stubs_v == 0) {
					++stubs_v;
				}

				size_t begin_v = stubs_so_far;
				size_t end_v = begin_v + stubs_v;
				std::fill(stubs.begin() + begin_v, stubs.begin() + end_v, v);

				stubs_so_far += stubs_v;
			}
		}
		max_idx = stubs_so_far - 1;

		// put neighbours of u at the end

		const neighbourhood& N = G.get_neighbours(u);
		for (size_t v : N) {
			size_t stubs_v = G.degree(v);
			if (stubs_v == 0) {
				++stubs_v;
			}

			size_t begin_v = stubs_so_far;
			size_t end_v = begin_v + stubs_v;
			std::fill(stubs.begin() + begin_v, stubs.begin() + end_v, v);

			stubs_so_far += stubs_v;
		}

		// stubs before creating m0 edges
		size_t stubs_u = G.degree(u);
		if (G.degree(u) == 0) {
			++stubs_u;
		}

		size_t begin_u = stubs_so_far;
		size_t end_u = begin_u + stubs_u;
		std::fill(stubs.begin() + begin_u, stubs.begin() + end_u, u);

		// Add new stubs
		if (G.n_nodes() - N.size() - 1 < m0) {
			m0 = G.n_nodes() - N.size() - 1;
		}
		size_t new_stubs_u = m0;
		if (G.degree(u) == 0) {
			--new_stubs_u;
		}

		stubs.insert(stubs.end(), new_stubs_u, u);
		return m0;
	}

	inline void add_source_vertex
	(const uugraph& G, size_t u, size_t m0, std::vector<size_t>& stubs)
	{
		if (G.degree(u) == 0) {
			stubs.insert(stubs.end(), m0 - 1, u);
		}
		else {
			stubs.insert(stubs.end(), m0, u);
		}
	}
} // -- namespace _pa

template<class G, typename dT>
void BA_no_vertex_growth
(
	utils::drandom_generator<G,dT>& rg,
	size_t n0, size_t m0, size_t T,
	uugraph& Gs
)
{
	// initialize sequences of graphs
	Gs.init(n0);

	// Initialize list of stubs to choose from. For every node, as
	// many stubs as the degree of the node. However, if the node
	// has degree 0, the list will have 1 stub for that node.
	std::vector<size_t> stubs(n0);
	for (size_t i = 0; i < stubs.size(); ++i) {
		stubs[i] = i;
	}

	// at each time step t (1 <= t <= T) add one vertex to G
	// connected to m0 vertices chosen following the preferential
	// attachment rule making sure that no vertex is chosen twice
	// in the same time step.
	for (size_t t = 1; t <= T; ++t) {

		// upper bound of the interval within which the
		// random numbers will be generated
		size_t max_idx = stubs.size() - 1;

		// choose a vertex randomly from the stubs and update them
		rg.init_uniform(0, max_idx);
		size_t u_idx = rg.get_uniform();
		size_t u = stubs[u_idx];

		// arrange the vector of stubs to allow drawing a node with
		// the appropriate probability
		size_t max_neigh = _ng::update_stubs_initial(Gs, u, m0, stubs, max_idx);

		// connect the vertex to m0 vertices in the graph
		for (size_t m = 0; m < max_neigh ; ++m) {
			// reset the uniform random generator
			rg.init_uniform(0, max_idx);

			// choose stub and make edge
			size_t stub_idx = rg.get_uniform();
			size_t v = stubs[stub_idx];

			// rearrange the vector stubs for next iteration
			_ng::update_stubs(Gs, stub_idx, stubs, max_idx);

			Gs.add_edge(u, v);
		}
	}
}

} // -- namespace random
} // -- namespace networks
} // -- namespace lgraph

