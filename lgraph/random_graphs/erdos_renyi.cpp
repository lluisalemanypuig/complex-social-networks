#include <lgraph/random_graphs/erdos_renyi.hpp>

namespace lgraph {
namespace networks {
namespace random {
namespace Erdos_Renyi {

	template<class G, typename dT>
	void erdos_renyi(drandom_generator<G,dT> *rg, size_t N, double p, uugraph& Gs) {
		Gs.init(N);

	}


} // -- namespace Erdos_Renyi
} // -- namespace random
} // -- namespace networks
} // -- namespace lgraph
