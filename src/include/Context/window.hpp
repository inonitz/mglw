#pragma once
#include "../base.hpp"
#include <array>




namespace Context::Window {
	void			   		  close();
	std::array<i32, 2> const& dimensions();
}