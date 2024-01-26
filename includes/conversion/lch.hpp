#ifndef UCG_LCH_HPP
#define UCG_LCH_HPP

#include "hex.hpp"

// -- C O L O R  N A M E S P A C E --------------------------------------------

namespace color {

	/* lch to hex */
	auto lch_to_hex(const double, const double, const double) -> color::hex;

} // namespace color

#endif // UCG_LCH_HPP
