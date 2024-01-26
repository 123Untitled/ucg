#ifndef COLOR_HEADER
#define COLOR_HEADER

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cmath>

using Lum		= double;
using Hexcolor	= uint32_t;
using Channel	= uint8_t;

class Color {

public:

	static Channel getR(const Hexcolor color);
	static Channel getG(const Hexcolor color);
	static Channel getB(const Hexcolor color);

	void test(void);
	double midRGB(void) const;
	Color HSLtoRGB(const double hue, const double saturation, const double luminance);
	enum Gamma {
		// sRGB gamma
		SRGB,
		// 2.2 gamma
		GAM22,
		// 1.8 gamma
		GAM18,
		// sizeof enum
		GAMMA_NBR
	};

	enum Chan {
		RED,
		GREEN,
		BLUE,
		CHANNEL_NBR
	};


	double chroma(void);
	double lightness(void);
	double svalue(void);
	double slightness(void);
	double minRGB(void) const;
	double maxRGB(void) const;

	Lum hue(void);
	Lum luminance(Gamma gamma = SRGB) const;
	Lum luminosity(void) const;

	double saturation(void) const;


	Color(const Hexcolor color);


	void display(void) const;



	/* default constructor */
	Color(void);
	/* copy constructor */
	Color(const Color& copy);
	/* move constructor */
	Color(Color&& move);
	/* destructor */
	~Color(void);
	/* assignment operator */
	Color& operator=(const Color& assign);
	/* move operator */
	Color& operator=(Color&& move);

private:

	Channel _r;
	Channel _g;
	Channel _b;

	double	_R;
	double	_G;
	double	_B;

	double _min;
	double _max;

	double _chroma;
	double _value; // brightness HSB / HSV
	double _lightness; // HSL
	double _hue;

	double _svalue; // saturation HSB / HSV
	double _slightness; // HSL


	static const double _gamma[GAMMA_NBR][CHANNEL_NBR];

};

#endif

class RGB
{
public:
	unsigned char R;
	unsigned char G;
	unsigned char B;

	RGB(unsigned char r, unsigned char g, unsigned char b)
	{
		R = r;
		G = g;
		B = b;
	}

	bool Equals(RGB rgb)
	{
		return (R == rgb.R) && (G == rgb.G) && (B == rgb.B);
	}
};

class HSL
{
public:
	int H;
	double S;
	double L;

	HSL(int h, double s, double l)
	{
		H = h;
		S = s;
		L = l;
	}

	bool Equals(HSL hsl)
	{
		return (H == hsl.H) && (S == hsl.S) && (L == hsl.L);
	}
};

static double HueToRGB(double v1, double v2, double vH) {
	if (vH < 0)
		vH += 1;

	if (vH > 1)
		vH -= 1;

	if ((6 * vH) < 1)
		return (v1 + (v2 - v1) * 6 * vH);

	if ((2 * vH) < 1)
		return v2;

	if ((3 * vH) < 2)
		return (v1 + (v2 - v1) * ((2.0 / 3) - vH) * 6);

	return v1;
}

static RGB HSLToRGB(HSL hsl) {
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	if (hsl.S == 0)
	{
		r = g = b = (unsigned char)(hsl.L * 255);
	}
	else
	{
		double v1, v2;
		double hue = (double)hsl.H / 360;

		v2 = (hsl.L < 0.5) ? (hsl.L * (1 + hsl.S)) : ((hsl.L + hsl.S) - (hsl.L * hsl.S));
		v1 = 2 * hsl.L - v2;

		r = (unsigned char)(255 * HueToRGB(v1, v2, hue + (1.0 / 3)));
		g = (unsigned char)(255 * HueToRGB(v1, v2, hue));
		b = (unsigned char)(255 * HueToRGB(v1, v2, hue - (1.0 / 3)));
	}

	return RGB(r, g, b);
}


inline double hue2rgb(double p, double q, double t) {
	if (t < 0) t += 1;
	if (t > 1) t -= 1;

	if (t < 1.0 / 6.0)
		return p + (q - p) * 6 * t;

	if (t < 1.0 / 2.0)
		return q;

	if (t < 2.0 / 3.0)
		return p + (q - p) * (2.0 / 3.0 - t) * 6;
	return p;
}

#include <cmath>
inline void hslToRgb(double h, double s, double l){
	double r, g, b;

	if (s == 0) {
		r = g = b = l; // achromatic
	}
	else {

		double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
		double p = 2 * l - q;
		r = hue2rgb(p, q, h + 1.0 / 3.0);
		g = hue2rgb(p, q, h);
		b = hue2rgb(p, q, h - 1.0 / 3.0);
	}

	r = std::round(r * 255);
	g = std::round(g * 255);
	b = std::round(b * 255);

	std::cout << "R-> " << r << std::endl;
	std::cout << "G-> " << g << std::endl;
	std::cout << "B-> " << b << std::endl;
}


