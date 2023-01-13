#include "window.hpp"
#include "state.hpp"
#include "glue.hpp"




namespace Context::Window {


	void close() {
		__internal::global().close();
		return;
	}


	std::array<i32, 2> const& dimensions() {
		return __internal::global().dims;
	}


}