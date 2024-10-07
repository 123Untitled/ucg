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




int main(int ac, char** av) {


	// TO REMOVE, JUSTE HERE FOR GENERATE GRADIENT FOR ANOTHER PROJECT
	double hue = 180;

	const int fd = open("/Users/untitled/Desktop/code/projects/https_server/server/include/gradient.hpp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1) {
		std::cerr << "error: " << std::strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}

	write(fd, "static ws::array _colors {\n", 27);

	while (hue < 360) {
		color::hex c = color::lch_to_hex(78, 58, hue);
		const auto e = c.escape();
		std::cout << e << std::endl;
		::write(fd, "\t\"", 2);
		::write(fd, e.data(), e.size());
		::write(fd, "\",\n", 3);
		++hue;
	}
	::write(fd, "};\n", 3);
	::close(fd);
	return 0;
	// END REMOVE



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
