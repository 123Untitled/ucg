#ifndef HSB_HPP
#define HSB_HPP


#include <cmath>
#include <cstdint>
#include <iostream>
#include <algorithm>

#include <cstdio>
#include "hex.hpp"
#include <unistd.h>

// -- C O L O R  N A M E S P A C E --------------------------------------------

namespace color {

	/* hsb to hex */
	auto hsb_to_hex(double, double, double) -> color::hex;

}


#endif
