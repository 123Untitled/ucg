#include "hsl.hpp"

static auto hue_to_rgb(double v1, double v2, double vh) -> double {
	if (vh < 0) vh += 1;

	if (vh > 1) vh -= 1;

	if ((6 * vh) < 1)
		return (v1 + (v2 - v1) * 6 * vh);

	if ((2 * vh) < 1)
		return v2;

	if ((3 * vh) < 2)
		return (v1 + (v2 - v1) * ((2.0 / 3) - vh) * 6);

	return v1;
}


auto color::hsl_to_hex(double hue, double saturation, double lightness) -> color::hex {

	hue        = hue        < 0 ? 0.0 : hue        > 360 ? 360.0 : hue;
	saturation = saturation < 0 ? 0.0 : saturation > 1.0 ? 1.0   : saturation;
	lightness  = lightness  < 0 ? 0.0 : lightness  > 1.0 ? 1.0   : lightness;

	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	if (saturation == 0.0) {
		r = g = b = static_cast<unsigned char>(lightness * 255);
		return color::hex{r, g, b};
	}

	double v1, v2;

	v2 = (lightness < 0.5) ? (lightness * (1 + saturation)) : ((lightness + saturation) - (lightness * saturation));
	v1 = (2 * lightness) - v2;
	hue /= 360;

	r = static_cast<unsigned char>(255 * hue_to_rgb(v1, v2, hue + (1.0 / 3)));
	g = static_cast<unsigned char>(255 * hue_to_rgb(v1, v2, hue));
	b = static_cast<unsigned char>(255 * hue_to_rgb(v1, v2, hue - (1.0 / 3)));

	return color::hex{r, g, b};
}


