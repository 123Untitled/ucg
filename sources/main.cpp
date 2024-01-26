#include "lch.hpp"
#include "hsl.hpp"
#include "hsb.hpp"
#include "colorscheme.hpp"

#include <iostream>
#include <filesystem>
#include <regex>
#include <fstream>

#include "alacritty.hpp"
#include "vim.hpp"

// toml++
#include <toml++/toml.hpp>


auto search(const std::string& dir, const std::string& reg) -> std::optional<std::string> {
	// get home directory
	static const std::string home = std::filesystem::absolute(std::filesystem::path(getenv("HOME"))).string();
	// make absolute path
	const std::string path = home + "/" + dir;
	// check if path exists
	if (std::filesystem::exists(path) == true) {
		// follow symlinks
		std::filesystem::directory_options options = std::filesystem::directory_options::follow_directory_symlink;
		// regex
		std::regex regex(reg);
		// recursive search
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path, options)) {
			// skip if not a file or symlink
			if (entry.is_regular_file() == false && entry.is_symlink() == false) {
				continue;
			} // check if file matches regex
			if (std::regex_match(entry.path().filename().string(), regex))
				// return absolute path
				return entry.path().string();
		}
	}
	return std::nullopt;
}




int main(void) {

	ucg::colorscheme scheme{};

	auto alacritty_yaml = ::search(".config/", "alacritty.ya?ml");
	auto alacritty_toml = ::search(".config/", "alacritty.to?ml");
	auto vim_scheme            = ::search(".config/", "ucg.vim");

	if (alacritty_yaml == std::nullopt) {
		std::cout << "alacritty config not found" << std::endl;
		return EXIT_FAILURE;
	}
	else {
		config::alacritty<config::type::YAML>::run(*alacritty_yaml, scheme);
	}

	if (alacritty_toml == std::nullopt) {
		std::cout << "alacritty config not found" << std::endl;
		return EXIT_FAILURE;
	}
	else {
		config::alacritty<config::type::TOML>::run(*alacritty_toml, scheme);
	}


	if (vim_scheme == std::nullopt) {
		std::cout << "vim scheme not found" << std::endl;
		return EXIT_FAILURE;
	}
	else {
		vim vim{*vim_scheme, scheme};
		vim.generate();
	}

	return EXIT_SUCCESS;




	double lightness = 80;
	double saturation = 60;

	double hue = 0.0;
	while (hue < 360) {
		color::lch_to_hex(lightness, saturation, hue);
		//color::hsb_to_hex(hue, saturation / 100.0, lightness / 100.0);
		//color::hsl_to_hex(hue, saturation / 100.0, lightness / 100.0);
		std::cout << std::endl;
		hue += 4;
	}

	return 0;
}
