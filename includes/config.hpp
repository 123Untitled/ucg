/*****************************************************************************/
/*                                                                           */
/*                 ░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░                   */
/*                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░                  */
/*                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░                         */
/*                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒▒▓███▓▒░                  */
/*                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░                  */
/*                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░                  */
/*                  ░▒▓██████▓▒░ ░▒▓██████▓▒░ ░▒▓██████▓▒░                   */
/*                                                                           */
/*****************************************************************************/

#pragma once

#ifndef UCG_CONFIG_HEADER
#define UCG_CONFIG_HEADER

#include "memory/memory.hpp"

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>


// -- U C G  N A M E S P A C E ------------------------------------------------

namespace ucg {


	// -- C O N F I G ---------------------------------------------------------

	class config final {


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = ucg::config;


			// -- public lifecycle --------------------------------------------

			/* deleted copy constructor */
			config(const self&) = delete;

			/* deleted move constructor */
			config(self&&) = delete;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public static methods ---------------------------------------

			/* targets */
			static auto targets(void) -> std::vector<std::string> {
				auto& instance = self::shared();
				return {};
			}


		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			config(void) {

				// get home directory
				const char* home = ::getenv("HOME");

				if (home == nullptr)
					throw std::runtime_error{"HOME environment variable not set"};

				constexpr char suffix[] = "/.config/ucg/ucg.toml";

				auto len = __builtin_strlen(home);

				// null-terminate is already included in suffix
				char* path = ucg::allocator::allocate<char>(len + sizeof(suffix));

				__builtin_memcpy(path, home, len);
				__builtin_memcpy(path + len, suffix, sizeof(suffix));


				// open file descriptor (read-only, non-blocking)
				const auto fd = ::open(path, O_RDONLY | O_NONBLOCK);

				if (fd == -1) {

					switch (errno) {
						// file not found
						case ENOENT:
							throw std::runtime_error{"ucg.toml not found"};
						// permission denied
						case EACCES:
							throw std::runtime_error{"permission denied"};
						// default
						default:
							throw std::runtime_error{"unknown error"};
					}
				}

				// get file size
				struct stat st;

				if (::fstat(fd, &st) == -1 || !S_ISREG(st.st_mode) || st.st_size < 0) {
					::close(fd);
					throw std::runtime_error{"fstat failed"};
				}

				const auto size = static_cast<unsigned long>(st.st_size);

				char* buffer = ucg::allocator::allocate<char>(size + 1);

				// read file
				const auto bytes = ::read(fd, buffer, size + 1);

				if (bytes == -1)
					throw std::runtime_error{"read failed"};

				// close file descriptor
				::close(fd);

				// null-terminate buffer
				buffer[size] = '\0';


				::write(STDOUT_FILENO, buffer, size);

			}

			/* destructor */
			~config(void) noexcept {
			}


			// -- private static methods --------------------------------------

			/* shared */
			static auto shared(void) -> self& {
				static self instance{};
				return instance;
			}


	}; // class config

} // namespace ucg

#endif // UCG_CONFIG_HEADER
