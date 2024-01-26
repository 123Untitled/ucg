#ifndef LCH_HPP
#define LCH_HPP

#include <cmath>
#include <stdio.h>
#include <unistd.h>

#include "hex.hpp"



// -- C O L O R  N A M E S P A C E --------------------------------------------

namespace color {

	/* lch to hex */
	auto lch_to_hex(const double, const double, const double) -> color::hex;

}



#endif
