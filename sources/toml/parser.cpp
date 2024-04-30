#include "toml/parser.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdexcept>
#include <new>


static const char* ct_debug[] {
	"nil",
	"ctrl",
	"whitespace",

	"newline",
	"return",

	"hash",

	"simple",
	"double",

	"equals",
	"plus",
	"minus",

	"dot",
	"colon",
	"exponent",
	"underscore",
	"number",
	"other",
	"extended",
	"backslash",
	"open_square",
	"close_square",
	"open_curly",
	"close_curly"
};




// -- public lifecycle --------------------------------------------------------

/* default constructor */
ucg::toml_parser::toml_parser(void) noexcept
: _path{nullptr}, _buffer{nullptr}, _size{0U} {
}

/* path constructor */
ucg::toml_parser::toml_parser(const char* const path)
: _path{path}, _buffer{nullptr}, _size{0U} {


	auto fd = ::open(path, O_RDONLY | O_NONBLOCK | O_SYMLINK);

	if (fd == -1)
		throw std::runtime_error("toml_parser: open failed");


	// get file size
	struct stat st;
	if (::fstat(fd, &st) == -1)
		throw std::runtime_error("toml_parser: fstat failed");

	// check is regular file
	if (!S_ISREG(st.st_mode))
		throw std::runtime_error("toml_parser: not a regular file");

	// check size
	if (st.st_size < 0)
		throw std::runtime_error("toml_parser: invalid file size");

	_size = static_cast<size_type>(st.st_size);

	// allocate buffer
	_buffer = static_cast<char*>(__builtin_malloc(_size));

	// check buffer
	if (_buffer == nullptr)
		throw std::runtime_error("toml_parser: malloc failed");

	// read file
	auto readed = ::read(fd, _buffer, _size);

	if (readed == -1)
		throw std::runtime_error("toml_parser: read failed");

	/*
		auto stype = STATE_INITIAL;
	*/

	::write(STDOUT_FILENO, "\x1b[34m", 5);
	::write(STDOUT_FILENO, _buffer, _size);
	::write(STDOUT_FILENO, "\x1b[0m", 4);

	for (size_type i = 0; i < _size; ++i) {

		auto ctype = self::chartype(_buffer[i]);

		::write(STDOUT_FILENO, "\x1b[32m", 5);
		::write(STDOUT_FILENO, ct_debug[ctype], __builtin_strlen(ct_debug[ctype]));
		::write(STDOUT_FILENO, " | \x1b[0m", 7);

		/*
			stype = self::statetype(stype, ctype);
		*/

	}

	__builtin_free(_buffer);

}


// -- private static methods --------------------------------------------------

/* char type */
auto ucg::toml_parser::chartype(const char character) noexcept -> char_type {

	// chartype table
	static constexpr char_type table[256] = {

		NIL, // Null character

		CTRL, // Start of Heading
		CTRL, // Start of Text
		CTRL, // End of Text
		CTRL, // End of Transmission
		CTRL, // Enquiry
		CTRL, // Acknowledge
		CTRL, // Bell, Alert
		CTRL, // Backspace

		WHITESPACE, // Horizontal Tab

		NEWLINE, // Line Feed

		CTRL, // Vertical Tabulation
		CTRL, // Form Feed

		RETURN, // Carriage Return

		CTRL, // Shift Out
		CTRL, // Shift In
		CTRL, // Data Link Escape
		CTRL, // Device Control One (XON)
		CTRL, // Device Control Two
		CTRL, // Device Control Three (XOFF)
		CTRL, // Device Control Four
		CTRL, // Negative Acknowledge
		CTRL, // Synchronous Idle
		CTRL, // End of Transmission Block
		CTRL, // Cancel
		CTRL, // End of medium
		CTRL, // Substitute
		CTRL, // Escape
		CTRL, // File Separator
		CTRL, // Group Separator
		CTRL, // Record Separator
		CTRL, // Unit Separator

		WHITESPACE, // Space

		OTHER, // Exclamation mark
		DOUBLE, // Double quotes (or speech marks)

		HASH, // Number sign (hash)

		OTHER, // Dollar
		OTHER, // Per cent sign
		OTHER, // Ampersand
		SIMPLE, // Single quote
		OTHER, // Open parenthesis (or open bracket)
		OTHER, // Close parenthesis (or close bracket)
		OTHER, // Asterisk

		PLUS, // Plus
		OTHER, // Comma
		MINUS, // Hyphen-minus

		DOT, // Period, dot or full stop
		OTHER, // Slash or divide

		NUMBER, // Zero
		NUMBER, // One
		NUMBER, // Two
		NUMBER, // Three
		NUMBER, // Four
		NUMBER, // Five
		NUMBER, // Six
		NUMBER, // Seven
		NUMBER, // Eight
		NUMBER, // Nine

		COLON, // Colon
		OTHER, // Semicolon
		OTHER, // Less than (or open angled bracket)
		EQUALS, // Equals
		OTHER, // Greater than (or close angled bracket)
		OTHER, // Question mark
		OTHER, // At sign
		OTHER, // Uppercase A
		OTHER, // Uppercase B
		OTHER, // Uppercase C
		OTHER, // Uppercase D
		EXPONENT, // Uppercase E
		OTHER, // Uppercase F
		OTHER, // Uppercase G
		OTHER, // Uppercase H
		OTHER, // Uppercase I
		OTHER, // Uppercase J
		OTHER, // Uppercase K
		OTHER, // Uppercase L
		OTHER, // Uppercase M
		OTHER, // Uppercase N
		OTHER, // Uppercase O
		OTHER, // Uppercase P
		OTHER, // Uppercase Q
		OTHER, // Uppercase R
		OTHER, // Uppercase S
		OTHER, // Uppercase T
		OTHER, // Uppercase U
		OTHER, // Uppercase V
		OTHER, // Uppercase W
		OTHER, // Uppercase X
		OTHER, // Uppercase Y
		OTHER, // Uppercase Z

		OPEN_SQUARE, // Opening bracket
		BACKSLASH, // Backslash
		CLOSE_SQUARE, // Closing bracket

		OTHER, // Caret - circumflex
		UNDERSCORE, // Underscore
		OTHER, // Grave accent

		OTHER, // Lowercase a
		OTHER, // Lowercase b
		OTHER, // Lowercase c
		OTHER, // Lowercase d

		EXPONENT, // Lowercase e

		OTHER, // Lowercase f
		OTHER, // Lowercase g
		OTHER, // Lowercase h
		OTHER, // Lowercase i
		OTHER, // Lowercase j
		OTHER, // Lowercase k
		OTHER, // Lowercase l
		OTHER, // Lowercase m
		OTHER, // Lowercase n
		OTHER, // Lowercase o
		OTHER, // Lowercase p
		OTHER, // Lowercase q
		OTHER, // Lowercase r
		OTHER, // Lowercase s
		OTHER, // Lowercase t
		OTHER, // Lowercase u
		OTHER, // Lowercase v
		OTHER, // Lowercase w
		OTHER, // Lowercase x
		OTHER, // Lowercase y
		OTHER, // Lowercase z

		OPEN_CURLY, // Opening brace
		OTHER, // Vertical bar
		CLOSE_CURLY, // Closing brace
		OTHER, // Equivalency sign - tilde

		CTRL, // Delete


		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,

		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,

		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,

		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED,
		EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED, EXTENDED

	}; // ascii

	return table[static_cast<unsigned char>(character)];
}



// -- T O K E N  L I S T ------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
ucg::toml_parser::token_list::token_list(void) noexcept
: _head{nullptr}, _next{&_head} {
}

/* move constructor */
ucg::toml_parser::token_list::token_list(token_list&& other) noexcept
: _head{other._head}, _next{other._next} {
	// invalidate other
	other._init();
}

/* destructor */
ucg::toml_parser::token_list::~token_list(void) noexcept {
	this->_free();
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto ucg::toml_parser::token_list::operator=(token_list&& other) noexcept -> token_list& {

	// check for self-assignment
	if (this == &other)
		return *this;

	// free current list
	this->_free();

	// move other list
	_head = other._head;
	_next = other._next;

	// invalidate other
	other._init();

	return *this;
}


// -- public methods ----------------------------------------------------------

/* push */
auto ucg::toml_parser::token_list::push(const char* data, size_type size, token_type type) -> void {
	// allocate new token
	*_next = static_cast<token*>(__builtin_malloc(sizeof(token)));
	// check token
	if (*_next == nullptr)
		throw std::runtime_error("toml_parser: malloc failed");
	// construct token
	new(*_next) token{data, size, type, nullptr};
	// assign next pointer
	_next = &(*_next)->next;
}


// -- private methods ---------------------------------------------------------

/* free */
auto ucg::toml_parser::token_list::_free(void) noexcept -> void {
	// get head token
	auto current = _head;
	// loop over tokens
	while (current != nullptr) {
		// get next token
		auto next = current->next;
		// free token
		__builtin_free(current);
		// move to next token
		current = next; }
}

/* init */
auto ucg::toml_parser::token_list::_init(void) noexcept -> void {
	_head = nullptr;
	_next = &_head;
}
