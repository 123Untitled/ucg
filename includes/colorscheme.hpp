#ifndef UCG_COLORSCHEME_HPP
#define UCG_COLORSCHEME_HPP

#include <array>
#include <random>
#include <cmath>
#include <algorithm>

#include "lch.hpp"
#include "hsb.hpp"
#include "hex.hpp"


namespace ucg {

	enum colortype : unsigned char {
		A1, A2, A3,
		B1, B2, B3,
		C1, C2, C3,
		G1, G2, G3, G4, G5, G6, G7,
		COLOR_NBR
	};

	class colorscheme final {

		public:

			colorscheme(void);
			~colorscheme(void) = default;


			// -- public accessors --------------------------------------------

			template <ucg::colortype type>
			auto get(void) const -> const color::hex& {
				static_assert(type < ucg::COLOR_NBR, "): invalid color type :(");
				return _colors[type];
			}

			// -- public static methods ---------------------------------------

			static inline auto A1(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::A1>();
			}

			static inline auto A2(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::A2>();
			}

			static inline auto A3(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::A3>();
			}

			static inline auto B1(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::B1>();
			}

			static inline auto B2(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::B2>();
			}

			static inline auto B3(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::B3>();
			}

			static inline auto C1(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::C1>();
			}

			static inline auto C2(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::C2>();
			}

			static inline auto C3(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::C3>();
			}

			static inline auto G1(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::G1>();
			}

			static inline auto G2(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::G2>();
			}

			static inline auto G3(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::G3>();
			}

			static inline auto G4(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::G4>();
			}

			static inline auto G5(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::G5>();
			}

			static inline auto G6(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::G6>();
			}

			static inline auto G7(const ucg::colorscheme& scheme) -> const color::hex& {
				return scheme.get<ucg::colortype::G7>();
			}





		private:

			// -- private methods ---------------------------------------------

			auto generate(void) -> void;

			auto random_hues(void) -> void;

			auto best_hue(void) const -> double;

			auto triad_hue(void) -> void;

			auto tetrad_hue(void) -> void;

			auto analogous_hue(const double) -> void;

			auto golden_ratio_hue(void) -> void;

			auto circular_distance(const double, const double) const -> double;


			// -- private members ---------------------------------------------

			/* colorscheme */
			std::array<color::hex, COLOR_NBR> _colors;

			/* hues */
			std::array<double, 3> _hues;

	};

}


#endif
