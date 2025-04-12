#include "conversion/hsb.hpp"

#include "types.hpp"

#include <cmath>


namespace ucg {

	template <typename T>
	auto clamp(const T& value, const T& min, const T& max) noexcept -> const T& {
		return (value < min) ? min : ((value > max) ? max : value);
	}
}

auto ucg::hsb_to_hex(double hue, double saturation, double brightness) -> color::hex {

	// clamp values
	hue        = ucg::clamp(hue,        0.0, 360.0);
	saturation = ucg::clamp(saturation, 0.0,   1.0);
	brightness = ucg::clamp(brightness, 0.0,   1.0);

	const unsigned range = static_cast<unsigned>(hue / 60.0);

	const double c = brightness * saturation;
	const double m = brightness - c;

	const double x = c * (1 - std::abs(std::fmod(hue / 60, 2) - 1));

	double r, g, b;

	switch (range) {

		case 0U:
			r = (c + m);
			g = (x + m);
			b =  m     ;
			break;
		case 1U:
			r = (x + m);
			g = (c + m);
			b =  m     ;
			break;
		case 2U:
			r =  m     ;
			g = (c + m);
			b = (x + m);
			break;
		case 3U:
			r =  m     ;
			g = (x + m);
			b = (c + m);
			break;
		case 4U:
			r = (x + m);
			g =  m     ;
			b = (c + m);
			break;
		case 5U:
			r = (c + m);
			g =  m     ;
			b = (x + m);
			break;
		default:
			r = 0.0;
			g = 0.0;
			b = 0.0;
			break;
	}

	r *= 255.0;
	g *= 255.0;
	b *= 255.0;

	return color::hex{
		static_cast<ucg::byte>(r + 0.5), // round result
		static_cast<ucg::byte>(g + 0.5), // round result
		static_cast<ucg::byte>(b + 0.5)  // round result
	};
}
