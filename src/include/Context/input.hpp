#pragma once
#include "../base.hpp"
#include <array>




namespace Context {


	namespace Input {
		enum class KEY {
			W,
			A,
			S,
			D,
			Q,
			R,
			F,
			G,
			Z,
			X,
			C,
			V,
			NUM0,
			NUM1,
			NUM2,
			NUM3,
			NUM4,
			NUM5,
			NUM6,
			NUM7,
			NUM8,
			NUM9
		};


		std::array<f64, 2> const& cursorPosition();
		bool 					  isKeyPressed (KEY keyEnum);
		bool                      isKeyReleased(KEY keyEnum);
	}

	
}