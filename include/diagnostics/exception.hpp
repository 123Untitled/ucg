#ifndef midx_diagnostic_exception_hpp
#define midx_diagnostic_exception_hpp

#include "midx/utility/types.hpp"
#include <errno.h>
#include <string.h>


// -- U X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- E X C E P T I O N ---------------------------------------------------

	class exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::exception;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			exception(void) noexcept = default;

			/* copy constructor */
			exception(const self&) noexcept = default;

			/* move constructor */
			exception(self&&) noexcept = default;

			/* destructor */
			virtual ~exception(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public interface --------------------------------------------

			/* what */
			virtual auto what(void) const noexcept -> const char* = 0;

	}; // class exception


	// -- S Y S T E M  E R R O R ----------------------------------------------

	class system_error final : public mx::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::system_error;


			// -- private members ---------------------------------------------

			/* errno */
			int _errno;

			/* buffer */
			char _buffer[4096U];


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			system_error(void) noexcept;

			/* where constructor */
			template <unsigned N>
			system_error(const char (&where)[N]) noexcept
			: _errno{errno} /* _buffer{} uninitialized */ {

				// buffer size
				constexpr auto buffer_size = sizeof(_buffer);

				// get errno string error
				const char* err_str = ::strerror(errno);

				// check error
				if (err_str == nullptr)
					err_str = "unknown error";

				// where length
				size_t w_len = N - (where[N - 1U] == '\0' ? 1U : 0U);

				// error length
				size_t err_len = ::strlen(err_str);

				// check out of bounds
				if (err_len + w_len + 3U > buffer_size)
					err_len = buffer_size - w_len - 3U;

				// offset pointer
				char* ptr = _buffer;

				// copy where
				::memcpy(ptr, where, w_len);
				ptr += w_len;

				// copy separator
				::memcpy(ptr, ": ", 2);
				ptr += 2;

				// copy error
				::memcpy(ptr, err_str, err_len);
				ptr += err_len;

				// null terminate
				*ptr = '\0';
			}

			/* copy constructor */
			system_error(const self&) noexcept = default;

			/* move constructor */
			system_error(self&&) noexcept = default;

			/* destructor */
			~system_error(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* what */
			auto what(void) const noexcept -> const char* override final;


			// -- public accessors --------------------------------------------

			/* code */
			auto code(void) const noexcept -> int;

	}; // class system_error



	// -- R U N T I M E  E R R O R --------------------------------------------

	class runtime_error final : public mx::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::runtime_error;


			// -- private members ---------------------------------------------

			/* what */
			char _buffer[4096U];


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			runtime_error(void) noexcept {
				::memcpy(_buffer, "unknown error", 14U);
			}


			void _append(const char* str, mx::size_t& offset) {

				const auto len = ::strlen(str);

				char* ptr = _buffer + offset;

				::memcpy(ptr, str, len);

				ptr += len;

				::memcpy(ptr, " ", 1U);

				offset += len + 1U;
			}

			template<typename... Ts>
			runtime_error(Ts&&... args) noexcept {
				size_t offset = 0;
				unsigned i = 0U;
				(_append(args, offset), ...);
				_buffer[offset] = '\0';
			}

			/* copy constructor */
			runtime_error(const self&) noexcept = default;

			/* move constructor */
			runtime_error(self&&) noexcept = default;

			/* destructor */
			~runtime_error(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* what */
			auto what(void) const noexcept -> const char* override final {
				return _buffer;
			}

	}; // class runtime_error

} // namespace mx

#endif // midx_diagnostic_exception_hpp
