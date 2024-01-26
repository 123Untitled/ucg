#include "colorscheme.hpp"

#include <iostream>
#include <filesystem>
#include <regex>

#include "alacritty.hpp"
#include "neovim.hpp"


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




int main(int ac, char** av) {

	ucg::colorscheme scheme{};
	if (ac == 1) {
		ucg::colorscheme::print(scheme);
	}
	else {
		if (std::string_view{av[1]} == "-s" || std::string_view{av[1]} == "--silent")
			;
		else {
			std::cout << "usage: " << av[0] << " [-s|--silent]" << std::endl;
			return EXIT_FAILURE;
		}
	}

	auto alacritty_yaml = ::search(".config/", "alacritty.ya?ml");
	auto alacritty_toml = ::search(".config/", "alacritty.to?ml");
	auto neovim_scheme  = ::search(".config/", "ucg.vim");

	if (alacritty_yaml == std::nullopt) {
		std::cout << "alacritty config not found" << std::endl;
		return EXIT_FAILURE;
	}
	if (alacritty_toml == std::nullopt) {
		std::cout << "alacritty config not found" << std::endl;
		return EXIT_FAILURE;
	}
	if (neovim_scheme == std::nullopt) {
		std::cout << "vim scheme not found" << std::endl;
		return EXIT_FAILURE;
	}

	config::alacritty<config::type::YAML>::run(*alacritty_yaml, scheme);
	config::alacritty<config::type::TOML>::run(*alacritty_toml, scheme);

	config::neovim vim{*neovim_scheme, scheme};
	vim.generate();

	return EXIT_SUCCESS;
}
