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

#ifndef TOML_PARSER_HEADER
#define TOML_PARSER_HEADER


// -- U C G  N A M E S P A C E ------------------------------------------------

namespace ucg {

	// value and key must be on the same line
	// value types : String, Integer, Float, Boolean
	// will not be implemented immediatly: Offset Date-Time, Local Date-Time, Local Date, Local Time, Array, Inline Table
	// Unspecified values are invalid.


	// -- T O M L  P A R S E R ------------------------------------------------

	class toml_parser final {

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = ucg::toml_parser;

			/* size type */
			using size_type = decltype(sizeof(0));


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			toml_parser(void) noexcept;

			/* path constructor */
			toml_parser(const char* const);


		private:

			// -- private enums -----------------------------------------------

			/* token type */
			enum token_type : size_type {

				TABLE,
				KEY,
				WHITE,
				LINE,
				STRING,
				INTEGER,
				FLOAT,
				BOOLEAN,

			};


			// -- private structs ---------------------------------------------

			/* token */
			struct token final {
				// -- members -------------------------------------------------
				const char* data;
				size_type   size;
				token_type  type;
				token*      next;
			}; // struct token


			/* token list */
			class token_list final {

				public:

					// -- public lifecycle ------------------------------------

					/* default constructor */
					token_list(void) noexcept;

					/* deleted copy constructor */
					token_list(const token_list&) = delete;

					/* move constructor */
					token_list(token_list&&) noexcept;

					/* destructor */
					~token_list(void) noexcept;


					// -- public assignment operators -------------------------

					/* deleted copy assignment operator */
					auto operator=(const token_list&) -> token_list& = delete;

					/* move assignment operator */
					auto operator=(token_list&&) noexcept -> token_list&;


					// -- public methods --------------------------------------

					/* push */
					auto push(const char*, size_type, token_type) -> void;


				private:

					// -- private methods -------------------------------------

					/* free */
					auto _free(void) noexcept -> void;

					/* init */
					auto _init(void) noexcept -> void;


					// -- private members -------------------------------------

					/* head */
					token* _head;

					/* next */
					token** _next;

			}; // struct token_list


			/* char type */
			enum char_type : size_type {
				NIL, // not valid in comments

				CTRL,

				WHITESPACE,

				NEWLINE,
				RETURN,

				HASH,

				SIMPLE,
				DOUBLE,

				EQUALS,
				PLUS,
				MINUS,
				DOT,
				COLON,
				EXPONENT,
				UNDERSCORE,

				NUMBER,

				OTHER,

				EXTENDED,

				BACKSLASH,

				OPEN_SQUARE,
				CLOSE_SQUARE,

				OPEN_CURLY,
				CLOSE_CURLY,

				CHARTYPE_MAX
			};


			/* state type */
			enum __state : size_type {

				INITIAL,
			};


			/* action type */
			using __action = auto (self::*)(void) noexcept -> void;


			/* transition */
			struct __transition final {
				// -- members -------------------------------------------------
				__state  state;
				__action action;
			}; // struct transition





			// need to handle inf +inf -inf nan +nan -nan
			// true false

			// in date: 1979-05-27 07:32:00Z
			// Z represents the UTC timezone


			// -- private static methods --------------------------------------

			/* char type */
			static auto chartype(const char) noexcept -> char_type;


			// -- private members ---------------------------------------------

			/* path */
			const char* _path;

			/* buffer */
			char* _buffer;

			/* size */
			size_type _size;


	}; // class toml_parser

} // namespace ucg

#endif // TOML_PARSER_HEADER
