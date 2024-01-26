#include "testcolor.hpp"

/* default constructor */
Color::Color(void)
: _r{0}, _g{0}, _b{0}, _R{0}, _G{0}, _B{0} {
	// code here...
}

#define DOUBLE(x) static_cast<double>(x)

/* hexcolor constructor */
Color::Color(const Hexcolor color)
:	_r{getR(color)}, _g{getG(color)}, _b{getB(color)},
	_R{DOUBLE(_r) / 255}, _G{DOUBLE(_g) / 255}, _B{DOUBLE(_b) / 255},

	_min{minRGB()},
	_max{maxRGB()},
	_chroma(chroma()),
	_value(_max),
	_lightness{lightness()},
	_hue{hue()},
	_svalue{svalue()},
	_slightness{slightness()} {
	// code here...
	display();
}

Channel Color::getR(const Hexcolor color) { return (color >> 16) & 0xff; }
Channel Color::getG(const Hexcolor color) { return (color >> 8) & 0xff; }
Channel Color::getB(const Hexcolor color) { return (color & 0xff); }

void Color::display(void) const {
	std::cout << std::setw(20) << "red: "			<< (int)_r << std::endl;
	std::cout << std::setw(20) << "green: "			<< (int)_g << std::endl;
	std::cout << std::setw(20) << "blue: "			<< (int)_b << std::endl;
	std::cout << std::setw(20) << "-------------"	<< std::endl;
	std::cout << std::setw(20) << "RED: "			<< _R << std::endl;
	std::cout << std::setw(20) << "GREEN: "			<< _G << std::endl;
	std::cout << std::setw(20) << "BLUE: "			<< _B << std::endl;
	std::cout << std::setw(20) << "-------------"	<< std::endl;
	std::cout << std::setw(20) << "LUMINANCE: "		<< luminance(GAM22) << std::endl;
	std::cout << std::setw(20) << "SATURATION: "	<< saturation() << std::endl;
	std::cout << std::setw(20) << "-------------"	<< std::endl;

	std::cout << std::setw(20) << "CHROMA: "		<< _chroma << std::endl;
	std::cout << std::setw(20) << "LIGHTNESS: "		<< _lightness << std::endl;
	std::cout << std::setw(20) << "HUE: "			<< _hue << std::endl;
	std::cout << std::setw(20) << "VALUE: "			<< _max << std::endl;
	std::cout << std::setw(20) << "SVALUE: "		<< _svalue << std::endl;
	std::cout << std::setw(20) << "SLIGHTNESS: "	<< _slightness << std::endl;
}

/* copy constructor */
Color::Color(const Color&) {}

/* move constructor */
Color::Color(Color&&) {}

/* destructor */
Color::~Color(void) {}

/* assignment operator */
Color& Color::operator=(const Color&) { return (*this); }

/* move operator */
Color& Color::operator=(Color&&) { return (*this); }

Lum Color::hue(void) {

	if (_chroma == 0.0) { _hue = 0.0; }

	else if (_R == _max)
		_hue = 60 * (0.0 + ((_G - _B) / _chroma));

	else if (_G == _max)
		_hue = 60 * (2.0 + ((_B - _R) / _chroma));

	else if (_B == _max)
		_hue = 60 * (4.0 + ((_R - _G) / _chroma));

	return _hue < 0 ? _hue += 360 : _hue;
}

double Color::saturation(void) const {
	return (maxRGB() - minRGB()) / (maxRGB());
}

Lum Color::luminance(Gamma gamma) const {
	// secure array out of range
	gamma = static_cast<Gamma>(gamma % GAMMA_NBR);
	// luminance formula
	return	(_R * _gamma[gamma][RED])
		+	(_G * _gamma[gamma][GREEN])
		+	(_B * _gamma[gamma][GREEN]);
}


const double Color::_gamma[GAMMA_NBR][CHANNEL_NBR] = {
	// sRGB
	{ 0.2126, 0.7152, 0.0722 },
	// 2.2 gamma
	{ 0.299, 0.587, 0.114 },
	// 1.8 gamma
	{ 0.2388, 0.7031, 0.0661 }
};



double Color::minRGB(void) const {
	double min;
	_R < _G ? _R < _B ? min = _R : min = _B :
		_G < _B ? min = _G : min = _B ;
	return min;
	/* or */ // min = value - chroma;
}

double Color::maxRGB(void) const {
	double max;
	_R > _G ? _R > _B ? max = _R : max = _B :
		_G > _B ? max = _G : max = _B ;
	return max;
	/* or */ // value = max;
}

double Color::midRGB(void) const {
	return (minRGB() + maxRGB()) / 2;
}


double Color::chroma(void) {
	return (_max - _min);
	/* or */
	// chroma = 2 * (value - lightness);
}

double Color::lightness(void) {
	return (midRGB());
	/* or */ // _lightness = _max - (_chroma / 2);
}

double Color::slightness(void) {
	if (_lightness == 0 || _lightness == 1)
		return 0.0;
	return (_value - _lightness) / _lightness < (1 - _lightness) ? _lightness : (1 - _lightness);
}


double Color::svalue(void) {
	if (_value == 0)
		return 0.0;
	return _chroma / _value;
}
