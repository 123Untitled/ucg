#ifndef UCG_HEX_HPP
#define UCG_HEX_HPP

#include <string>


// -- C O L O R  N A M E S P A C E --------------------------------------------

namespace color {


	class hex final {

		public:

			using ubyte = unsigned char;

			/* deleted default constructor */
			inline constexpr hex(void) noexcept
			: _hex{} {}

			/* rgb constructor */
			inline constexpr hex(const ubyte r, const ubyte g, const ubyte b) noexcept 
			: _hex{'#', _table[r >> 4], _table[r & 0x0F],
						_table[g >> 4], _table[g & 0x0F],
						_table[b >> 4], _table[b & 0x0F], '\0'} {
			}

			/* copy constructor */
			inline constexpr hex(const color::hex& other) noexcept 
			: _hex{other._hex[0], other._hex[1], other._hex[2],
				   other._hex[3], other._hex[4], other._hex[5],
				   other._hex[6], other._hex[7]} {}

			/* default move constructor */
			inline constexpr hex(hex&&) noexcept = default;

			/* destructor */
			inline constexpr ~hex(void) noexcept {}


			// -- public assignment operators ----------------------------------

			/* copy assignment operator */
			inline constexpr auto operator=(const color::hex& other) noexcept -> color::hex& {
				_hex[0] = other._hex[0];
				_hex[1] = other._hex[1];
				_hex[2] = other._hex[2];
				_hex[3] = other._hex[3];
				_hex[4] = other._hex[4];
				_hex[5] = other._hex[5];
				_hex[6] = other._hex[6];
				_hex[7] = other._hex[7];
				return *this;
			}

			/* move assignment operator */
			inline constexpr auto operator=(color::hex&&) noexcept -> color::hex& = default;


			/* conversion operator */
			inline constexpr operator const char*(void) const noexcept {
				return _hex;
			}

			/* const data */
			inline constexpr auto data(void) const noexcept -> const char* {
				return _hex;
			}


		private:

			/* hex string */
			char _hex[8];

			/* char table */
			static constexpr char _table[] = "0123456789abcdef";

	};

}

#endif // UCG_HEX_HPP
