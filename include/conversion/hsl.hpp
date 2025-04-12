#ifndef HSL_HPP
#define HSL_HPP

#include "hex.hpp"

#include <stdio.h>
#include <iostream>
#include <unistd.h>

// -- C O L O R  N A M E S P A C E --------------------------------------------

namespace color {

	/* hsl to hex */
	auto hsl_to_hex(double, double, double) -> color::hex;

}



#endif
