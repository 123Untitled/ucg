#include "colorscheme.hpp"

#include <iostream>
#include <filesystem>
#include <regex>

#include "config/alacritty.hpp"
#include "config/neovim.hpp"

#include "config.hpp"
#include "memory/memory.hpp"

#include "toml/parser.hpp"


template <decltype(sizeof(0)) N>
auto error(const char (&msg)[N]) -> ::ssize_t {
	return ::write(STDERR_FILENO, msg, N);
}


auto search(const std::string& dir, const std::string& reg) -> std::optional<std::string> {

	// get home directory
	static const std::string home{[]() -> std::string {
		const char* home_env = ::getenv("HOME");
		return std::string{home_env == nullptr ? "" : home_env};
	}()};

	// make search path
	const std::string path = home + "/" + dir;

	// check if path exists
	if (std::filesystem::exists(path) == false)
		return std::nullopt;

	// follow symlinks option
	std::filesystem::directory_options options = std::filesystem::directory_options::follow_directory_symlink;

	// regex
	std::regex regex{reg};

	// recursive search
	for (const auto& entry : std::filesystem::recursive_directory_iterator(path, options)) {
		// skip if not a file
		if (entry.is_regular_file() == false)
			continue;
		// check if file matches regex
		if (std::regex_match(entry.path().filename().string(), regex))
			// return absolute path
			return entry.path().string();
	}
	return std::nullopt;
}

#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>




auto export_scheme(const ucg::colorscheme& scheme) -> void {

	// check if config directory exists
	struct ::stat st;

	// get home
	struct ::passwd* pw = getpwuid(getuid());

	if (pw == nullptr || pw->pw_dir == nullptr) {
		error("failed to get home directory\n");
		return;
	}

	char home[PATH_MAX];

	for (unsigned i = 0U; true; ++i) {

		if (i >= PATH_MAX) {
			error("home directory too long\n");
			return;
		}

		if (pw->pw_dir[i] == '\0') {

			constexpr char prefix[] = "/.config/ucg";

			if (i + sizeof(prefix) > PATH_MAX) {
				error("home directory too long\n");
				return;
			}

			__builtin_memcpy(home + i, prefix, sizeof(prefix));
			break;
		}

		home[i] = pw->pw_dir[i];
	}




	if (::stat(home, &st) == -1) {

		if (errno != ENOENT) {
			error("failed to get config directory\n");
			return;
		}

		// create directory
		if (::mkdir(home, 0755) == -1) {
			error("failed to create config directory\n");
			return;
		}

		error("config directory not found\n");
		return;
	}

	if (S_ISDIR(st.st_mode) == false) {
		error("config directory is not a directory\n");
	}



}



auto main(int ac, char** av) -> int {

	//ucg::hsb_to_hex(285.111, 0.0, 1.0);
	//
	//
	//return 0;

	//double d = 1.0;
	//
	//unsigned int b = static_cast<unsigned int>(d * 255.0);
	//
	//std::cout << "b: " << b << std::endl;
	//return 0;




	//double h = 210.0f; // teinte (ex: 0=rouge, 120=vert, 240=bleu)
 //   double s = 0.0f; // saturation (0 à 1)
 //   double b = 1.0f;  // luminosité (0 à 1)
	//
 //   RGB rgb = hsbToRgb(h, s, b);
 //   printf("RGB = (%d, %d, %d)\n", rgb.r, rgb.g, rgb.b);
	//
 //   return 0;

	//ucg::hsb_to_hex(0.0, 0.5, 1.0);
	//return 0;


	// toml parser: Control characters other than tab (U+0000 to U+0008, U+000A to U+001F, U+007F) are not permitted in comments.

	// mean: null to backspace, newline to unit separator, delete


	// U+0000 to U+0008, U+000A to U+001F, U+007F

	//ucg::toml_parser parser{"/Users/untitled/.config/ucg/ucg.toml"};

	//return 0;



	/*
	try {

		//ucg::memory<1>::allocate<int>(5);

		ucg::config::targets();

	} catch (const std::exception& e) {
		
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}


	return 0;
	*/


	ucg::colorscheme scheme{};


	if (ac == 1)
		ucg::colorscheme::print(scheme);

	else {
		if (std::string_view{av[1]} == "-s" || std::string_view{av[1]} == "--silent")
			;
		else {
			std::cout << "usage: " << av[0] << " [-s|--silent]" << std::endl;
			return EXIT_FAILURE;
		}
	}
	return 0;

	//auto alacritty_yaml = ::search(".config/", "alacritty.ya?ml");
	auto alacritty_toml = ::search(".config/", "alacritty.to?ml");
	auto neovim_scheme  = ::search(".config/", "ucg.vim");

	//if (alacritty_yaml == std::nullopt) {
	//	std::cout << "alacritty config not found" << std::endl;
	//	return EXIT_FAILURE;
	//}
	if (alacritty_toml == std::nullopt) {
		error("alacritty config not found\n");
		return EXIT_FAILURE;
	}
	if (neovim_scheme == std::nullopt) {
		error("vim scheme not found\n");
		return EXIT_FAILURE;
	}

	//config::alacritty<config::type::YAML>::run(*alacritty_yaml, scheme);
	config::alacritty<config::type::TOML>::run(*alacritty_toml, scheme);

	config::neovim vim{*neovim_scheme, scheme};
	vim.generate();

	return EXIT_SUCCESS;
}
