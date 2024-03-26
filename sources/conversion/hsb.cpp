#include "conversion/hsb.hpp"


auto color::hsb_to_hex(double hue, double saturation, double brightness) -> color::hex {

	hue = std::clamp(hue, 0.0, 360.0);

	unsigned range = static_cast<unsigned>(std::floor(hue / 60));

	double c = brightness * saturation;
	double x = c * (1 - std::abs(std::fmod(hue / 60, 2) - 1));

	double m = brightness - c;

	double r, g, b;

	switch (range) {

		case 0:
			r = (c + m) * 255;
			g = (x + m) * 255;
			b =  m      * 255;
			break;
		case 1:
			r = (x + m) * 255;
			g = (c + m) * 255;
			b =  m      * 255;
			break;
		case 2:
			r =  m      * 255;
			g = (c + m) * 255;
			b = (x + m) * 255;
			break;
		case 3:
			r =  m      * 255;
			g = (x + m) * 255;
			b = (c + m) * 255;
			break;
		case 4:
			r = (x + m) * 255;
			g =  m      * 255;
			b = (c + m) * 255;
			break;
		case 5:
			r = (c + m) * 255;
			g =  m      * 255;
			b = (x + m) * 255;
			break;
		default:
			r = 0; g = 0; b = 0;
			break;
	}

	uint8_t _r = static_cast<uint8_t>(std::clamp(std::floor(r), 0.0, 255.0));
	uint8_t _g = static_cast<uint8_t>(std::clamp(std::floor(g), 0.0, 255.0));
	uint8_t _b = static_cast<uint8_t>(std::clamp(std::floor(b), 0.0, 255.0));

	//char buffer[256];
	//
	//int size = snprintf(buffer, sizeof(buffer),
	//		"\x1b[48;2;%d;%d;%dm                \x1b[0m\n",
	//		_r, _g, _b);
	//
	//if (size > 0) ::write(1, buffer, static_cast<size_t>(size));

	return color::hex{
		static_cast<uint8_t>(r),
		static_cast<uint8_t>(g),
		static_cast<uint8_t>(b)
	};
}











