
namespace dsa {
namespace utils {

template<class G, typename dT>
drandom_generator<G,dT>::drandom_generator() : random_generator<G,dT>() { }

template<class G, typename dT>
drandom_generator<G,dT>::~drandom_generator() { }

/* Initialisers of the distributions */

template<class G, typename dT>
void drandom_generator<G,dT>::init_uniform(dT a, dT b) {
	U = uniform_int_distribution<dT>(a, b);
}

/* Get pseudo-random numbers */

template<class G, typename dT>
dT drandom_generator<G,dT>::get_uniform() {
	dT r = U(random_generator<G,dT>::gen);
	return r;
}
	
} // -- namespace utils
} // -- namespace dsa

