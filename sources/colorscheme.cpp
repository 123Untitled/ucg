#include "colorscheme.hpp"

ucg::colorscheme::colorscheme(void)
: _colors{}, _hues{} {
	this->generate();
}

// get min distance between 2 hues
auto ucg::colorscheme::circular_distance(const double a, const double b) const -> double {
	const double diff = std::abs(a - b);
	return std::min(diff, 360 - diff);
}


// compute best hue
auto ucg::colorscheme::best_hue(void) const -> double {
	// get minimum circular distance
	const double d1 = circular_distance(_hues[0], _hues[1]);
	const double d2 = circular_distance(_hues[1], _hues[2]);
	const double d3 = circular_distance(_hues[2], _hues[0]);

	// distances added
	const double dst[3] = { (d1 + d3), (d2 + d1), (d3 + d2) };

	return dst[0] < dst[1] ?
		  (dst[0] < dst[2] ? _hues[0] : _hues[2])
		: (dst[1] < dst[2] ? _hues[1] : _hues[2]);
}



auto ucg::colorscheme::random_hues(void) -> void {

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis(0.0, 360.0);

	_hues[0] = dis(gen);
	_hues[1] = dis(gen);
	_hues[2] = dis(gen);
}

auto ucg::colorscheme::triad_hue(void) -> void {

	std::random_device rd;
	std::knuth_b gen(rd());
	//std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis(0.0, 360.0);

	_hues[0] = dis(gen);
	_hues[1] = std::fmod(_hues[0] + 120, 360);
	_hues[2] = std::fmod(_hues[1] + 120, 360);
}

auto ucg::colorscheme::tetrad_hue(void) -> void {

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis(0.0, 360.0);

	_hues[0] = dis(gen);
	_hues[1] = std::fmod(_hues[0] + 90, 360);
	_hues[2] = std::fmod(_hues[1] + 90, 360);
}



auto ucg::colorscheme::analogous_hue(const double step) -> void {

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis(0.0, 360.0);

	_hues[0] = dis(gen);
	_hues[1] = std::fmod(_hues[0] + step, 360);
	_hues[2] = std::fmod(_hues[1] + step, 360);
}

auto ucg::colorscheme::golden_ratio_hue(void) -> void {

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis(0.0, 360.0);

	_hues[0] = dis(gen);
	_hues[1] = std::fmod(_hues[0] + 222.5, 360);
	_hues[2] = std::fmod(_hues[1] + 222.5, 360);
}


static auto random(double min, double max) -> double {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis(min, max);

	return dis(gen);
}



auto ucg::colorscheme::generate(void) -> void {

	const double luma     = 70.0;
	const double chroma   = 60.0;
	double contrast = 15.0;

	this->random_hues();
	//this->triad_hue();
	//this->tetrad_hue();
	//this->analogous_hue(70);
	//this->golden_ratio_hue();

	//_hues[1] = _hues[0];
	//_hues[2] = _hues[0];

	
	std::size_t h = 0;
	for (std::size_t i = colortype::A1; i < colortype::G1; i += 3) {

		_colors[i    ] = color::lch_to_hex(luma + contrast, chroma, _hues[h]);
		_colors[i + 1] = color::lch_to_hex(luma,            chroma, _hues[h]);
		_colors[i + 2] = color::lch_to_hex(luma - contrast, chroma, _hues[h]);

		++h;
	}

	const double hue = this->best_hue();



	contrast = random(55, 85);

	const double min = 0.5 - contrast / 200;
	const double max = 0.86;
	//const double max = 0.5 + contrast / 200;
	double saturation = 0.10;

	const double increment = (max - min) / (6);


	std::size_t s = 0;
	double brightness = min;
	double steps = 6;

	for (std::size_t i = colortype::G1; i <= colortype::G7; ++i) {

		double increment = (max - brightness) / steps;

		_colors[i] = color::hsb_to_hex(hue, saturation, brightness);
		//_colors[i] = color::lch_to_hex(brightness * 100, saturation * 100, hue);

		brightness += increment;
		--steps;
	}

}

