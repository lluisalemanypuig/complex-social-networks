// C++ inlcudes
#include <iostream>
#include <vector>
using namespace std;

// lgraph includes
#include <lgraph/data_structures/uugraph.hpp>
#include <lgraph/data_structures/node_path.hpp>
#include <lgraph/graph_traversal/traversal.hpp>
#include <lgraph/io/io.hpp>
using namespace lgraph;

// custom includes
#include "src/definitions.hpp"
#include "src/io_wrapper.hpp"

namespace exe_tests {

err_type uu_path_all_all__single(ifstream& fin) {
	string input_graph, format;
	size_t n;

	// parse input field
	string field;
	fin >> field;
	if (field != "INPUT") {
		cerr << ERROR("uu_path_all_all__single.cpp", "uu_path_all_all__single") << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}
	fin >> n;
	if (n != 1) {
		cerr << ERROR("uu_path_all_all__single.cpp", "uu_path_all_all__single") << endl;
		cerr << "    Only one input file is allowed in this test." << endl;
		cerr << "    Instead, " << n << " were specified." << endl;
		return err_type::test_format_error;
	}
	fin >> input_graph >> format;

	// parse body field
	fin >> field;
	if (field != "BODY") {
		cerr << ERROR("uu_path_all_all__single.cpp", "uu_path_all_all__single") << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	uugraph G;
	err_type r = io_wrapper::read_graph(input_graph, format, &G);
	if (r != err_type::no_error) {
		if (r == err_type::io_error) {
			cerr << ERROR("uu_path_all_all__single.cpp", "uu_path_all_all__single") << endl;
			cerr << "    Could not open file '" << input_graph << "'" << endl;
		}
		else if (r == err_type::graph_format_error) {
			cerr << ERROR("uu_path_all_all__single.cpp", "uu_path_all_all__single") << endl;
			cerr << "    Format '" << format << "' not supported." << endl;
		}
		return r;
	}

	// this test needs no BODY

	vector<vector< node_path<_new_> > > uv_path;
	traversal::uxpath(&G, uv_path);
	for (node u = 0; u < G.n_nodes(); ++u) {
		for (node v = 0; v < G.n_nodes(); ++v) {
			cout << "(" << u << ", " << v << "): ";
			if (uv_path[u][v].size() == 0) {
				cout << "No path" << endl;
			}
			else {
				cout << uv_path[u][v].to_string() << "; "
					 << uv_path[u][v].get_length() << endl;
			}
		}
	}

	return err_type::no_error;
}

}
