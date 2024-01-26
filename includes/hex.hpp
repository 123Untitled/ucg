#ifndef UCG_HEX_HPP
#define UCG_HEX_HPP

#include <string>
#include <iostream>


// -- C O L O R  N A M E S P A C E --------------------------------------------

namespace color {


	class hex final {

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = color::hex;

			using byte = unsigned char;


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			inline constexpr hex(void) noexcept
			: _hex{} {}

			/* rgb constructor */
			inline constexpr hex(const byte r, const byte g, const byte b) noexcept 
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


			// -- public conversion operators ---------------------------------

			/* conversion operator */
			inline constexpr operator const char*(void) const noexcept {
				return _hex;
			}

			/* const data */
			inline constexpr auto data(void) const noexcept -> const char* {
				return _hex;
			}



			// -- public methods ----------------------------------------------

			/* escape sequence (for printing) */
			inline constexpr auto escape(void) const -> std::string {

				// convert red [1, 2]
				auto r  = self::hex_to_byte(_hex[1]) << 4;
					 r |= self::hex_to_byte(_hex[2]);

				// convert green [3, 4]
				auto g  = self::hex_to_byte(_hex[3]) << 4;
					 g |= self::hex_to_byte(_hex[4]);

				// convert blue [5, 6]
				auto b  = self::hex_to_byte(_hex[5]) << 4;
					 b |= self::hex_to_byte(_hex[6]);

				std::string escape;
				escape.resize(19);
				escape[0 ] = '\x1b';
				escape[1 ] = '[';
				escape[2 ] = '4';
				escape[3 ] = '8';
				escape[4 ] = ';';
				escape[5 ] = '2';
				escape[6 ] = ';';

				// red indexes: [7, 8, 9]
				escape[7 ] = '0' + (char) (r / 100);
				escape[8 ] = '0' + (char)((r / 10) % 10);
				escape[9 ] = '0' + (char) (r % 10);
				escape[10] = ';';
				// green indexes: [11, 12, 13]
				escape[11] = '0' + (char) (g / 100);
				escape[12] = '0' + (char)((g / 10) % 10);
				escape[13] = '0' + (char) (g % 10);
				escape[14] = ';';
				// blue indexes: [15, 16, 17]
				escape[15] = '0' + (char) (b / 100);
				escape[16] = '0' + (char)((b / 10) % 10);
				escape[17] = '0' + (char) (b % 10);
				escape[18] = 'm';

				return escape;
			}

			/* print hex */
			inline auto print(void) const noexcept -> void {
				auto buffer = this->escape();
				std::cout.write(buffer.data(), (std::streamsize)buffer.size());
				std::cout << "       \x1b[0m " << _hex << std::endl;
			}



		private:

			// -- private methods ---------------------------------------------

			/* hex char to byte */
			inline static constexpr auto hex_to_byte(const char c) noexcept -> byte {
				if (c >= '0' && c <= '9')
					return static_cast<byte>(c - '0');
				if (c >= 'a' && c <= 'f')
					return static_cast<byte>(c - 'a' + 10);
				if (c >= 'A' && c <= 'F')
					return static_cast<byte>(c - 'A' + 10);
				return 0;
			}


			/* hex string */
			char _hex[8];

			/* char table */
			static constexpr char _table[] = "0123456789abcdef";

	};

}

#endif // UCG_HEX_HPP
