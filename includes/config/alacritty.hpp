#ifndef UCG_ALACRITTY_HPP
#define UCG_ALACRITTY_HPP

#include "colorscheme.hpp"

#include "yaml-cpp/yaml.h"
#include "toml++/toml.hpp"

#include <fstream>
#include <iostream>
#include <string>


// -- C O N F I G  N A M E S P A C E ------------------------------------------

namespace config {

	// -- enums ---------------------------------------------------------------

	/* config type */
	enum class type : unsigned int {
		YAML, TOML
	};

	/* alacritty */
	template <type T>
	class alacritty final {

		// -- assertions ------------------------------------------------------

		/* config type */
		static_assert(T == type::YAML || T == type::TOML,
			"invalid config type"
		);

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = config::alacritty<T>;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			alacritty(void) = delete;

			/* deleted copy constructor */
			alacritty(const alacritty&) = delete;

			/* deleted move constructor */
			alacritty(alacritty&&) noexcept = delete;

			/* destructor */
			~alacritty(void) = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			alacritty& operator=(const alacritty&) = delete;

			/* deleted move assignment operator */
			alacritty& operator=(alacritty&&) noexcept = delete;


			// -- public static methods ---------------------------------------

			/* run */
			static auto run(const std::string& path,
							const ucg::colorscheme& scheme) -> void {
				// create config
				self{path, scheme};
			}


		private:

			// -- private types -----------------------------------------------

			/* underlying type */
			using underlying = std::conditional_t<T == type::YAML,
												  YAML::Node,
												  toml::table>;

			// -- private lifecycle -------------------------------------------

			/* path and colorscheme constructor (yaml) */
			alacritty(const std::string& path,
					  const ucg::colorscheme& scheme) requires (T == type::YAML)
			: _config{} {

				try {
					_config = YAML::LoadFile(path);

					YAML::Node colors  = _config["colors"];

					YAML::Node primary = colors["primary"];

					primary["background"] = scheme.get<ucg::colortype::G1>().data();
					primary["foreground"] = scheme.get<ucg::colortype::G7>().data();

					YAML::Node normal  = colors["normal"];

					normal["black"  ] = scheme.get<ucg::colortype::G1>().data();
					normal["red"    ] = scheme.get<ucg::colortype::A2>().data();
					normal["green"  ] = scheme.get<ucg::colortype::B2>().data();
					normal["yellow" ] = scheme.get<ucg::colortype::C2>().data();
					normal["blue"   ] = scheme.get<ucg::colortype::A1>().data();
					normal["magenta"] = scheme.get<ucg::colortype::B1>().data();
					normal["cyan"   ] = scheme.get<ucg::colortype::C1>().data();
					normal["white"  ] = scheme.get<ucg::colortype::G7>().data();

					YAML::Node bright  = colors["bright"];

					bright["black"  ] = scheme.get<ucg::colortype::G2>().data();
					bright["red"    ] = scheme.get<ucg::colortype::G3>().data();
					bright["green"  ] = scheme.get<ucg::colortype::G4>().data();
					bright["yellow" ] = scheme.get<ucg::colortype::G5>().data();
					bright["blue"   ] = scheme.get<ucg::colortype::G6>().data();
					bright["magenta"] = scheme.get<ucg::colortype::A3>().data();
					bright["cyan"   ] = scheme.get<ucg::colortype::B3>().data();
					bright["white"  ] = scheme.get<ucg::colortype::C3>().data();


					std::ofstream file{path};

					if (file.is_open()) {
						file << _config;
						file.close();
					}

				}
				catch (const YAML::Exception& e) {
					std::cerr << "error: " << e.what() << std::endl;
				}

			}

			/* path and colorscheme constructor (toml) */
			alacritty(const std::string& path,
					  const ucg::colorscheme& scheme) requires (T == type::TOML)
			: _config{} {

				try {
					_config = toml::parse_file(path);

					const color::hex& hex = ucg::colorscheme::G1(scheme);
					std::string str{hex};

					toml::table* colors = _config["colors"].as_table();

					if (colors == nullptr)
						colors = _config.insert("colors", toml::table{}).first->second.as_table();

					colors->insert_or_assign("primary", toml::table{
						{"background", ucg::colorscheme::G1(scheme).data()},
						{"foreground", ucg::colorscheme::G7(scheme).data()}
					});

					colors->insert_or_assign("normal", toml::table{
						{"black"  , ucg::colorscheme::G1(scheme).data()},
						{"red"    , ucg::colorscheme::A2(scheme).data()},
						{"green"  , ucg::colorscheme::B2(scheme).data()},
						{"yellow" , ucg::colorscheme::C2(scheme).data()},
						{"blue"   , ucg::colorscheme::A1(scheme).data()},
						{"magenta", ucg::colorscheme::B1(scheme).data()},
						{"cyan"   , ucg::colorscheme::C1(scheme).data()},
						{"white"  , ucg::colorscheme::G7(scheme).data()}
					});

					colors->insert_or_assign("bright", toml::table{
						{"black"  , ucg::colorscheme::G2(scheme).data()},
						{"red"    , ucg::colorscheme::G3(scheme).data()},
						{"green"  , ucg::colorscheme::G4(scheme).data()},
						{"yellow" , ucg::colorscheme::G5(scheme).data()},
						{"blue"   , ucg::colorscheme::G6(scheme).data()},
						{"magenta", ucg::colorscheme::A3(scheme).data()},
						{"cyan"   , ucg::colorscheme::B3(scheme).data()},
						{"white"  , ucg::colorscheme::C3(scheme).data()}
					});

					std::ofstream file{path};

					if (file.is_open()) {
						file << _config;
						file.close();
					}

				}
				catch (const toml::parse_error& err) {

					std::cerr << "Error parsing file '" << *err.source().path
							  << "':\n" << err.description()
							  << "\n (" << err.source().begin << ")\n";
				}

				catch (const std::exception& e) {
					std::cerr << "error: " << e.what() << std::endl;
				}

			}


			// -- private members ---------------------------------------------

			/* underlying */
			underlying _config;

	};

}

#endif
