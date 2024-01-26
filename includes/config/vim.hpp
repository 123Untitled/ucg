#ifndef UCG_VIM_HPP
#define UCG_VIM_HPP

#include <string>
#include <array>
#include <fstream>
#include <unordered_map>

#include "hex.hpp"
#include "colorscheme.hpp"


inline auto test(void) noexcept -> void {
}

class vim final {

	private:

		class highlight final {

			public:

				inline constexpr highlight(void)
				: _foreground{}, _background{}, _style{} {}

				highlight(const std::string& fg, const std::string& bg, const std::string_view& style)
				: _foreground{fg}, _background{bg}, _style{style} {}

				highlight(const color::hex& fg, const color::hex& bg, const std::string_view& style)
				: _foreground{fg}, _background{bg}, _style{style} {}

				highlight( const std::string& fg, const color::hex& bg, const std::string_view& style)
				: _foreground{fg}, _background{bg}, _style{style} {}

				highlight(const color::hex& fg, const std::string& bg, const std::string_view& style)
				: _foreground{fg}, _background{bg}, _style{style} {}


				/* copy constructor */
				inline constexpr highlight(const highlight& other) noexcept
				:	_foreground{other._foreground},
					_background{other._background},
					_style{other._style} {}

				/* move constructor */
				inline constexpr highlight(highlight&& other) noexcept
				:	_foreground{std::move(other._foreground)},
					_background{std::move(other._background)},
					_style{std::move(other._style)} {}


				// -- public assignment operators -----------------------------

				/* copy assignment operator */
				inline auto operator=(const highlight& other) noexcept -> highlight& {
					_foreground = other._foreground;
					_background = other._background;
						 _style = other._style;
					return *this;
				}

				/* move assignment operator */
				inline auto operator=(highlight&& other) noexcept -> highlight& {
					_foreground = std::move(other._foreground);
					_background = std::move(other._background);
						 _style = std::move(other._style);
					return *this;
				}


				// -- public accessors ----------------------------------------

				inline auto foreground(void) const -> const std::string& {
					return _foreground;
				}

				inline auto background(void) const -> const std::string& {
					return _background;
				}

				inline auto style(void) const -> const std::string_view& {
					return _style;
				}

			private:

				std::string      _foreground;
				std::string      _background;
				std::string_view _style;
		};




		template <typename... A> requires (sizeof...(A) == 3)
		auto add(const std::string_view group, A&&... args) -> void {
			_highlights.try_emplace(group, std::forward<A>(args)...);
		}

		auto add(const std::string_view& group, const std::string_view& target) -> void {
			_highlights.try_emplace(group, _highlights[target]);
		}


	public:

		/* deleted default constructor */
		vim(void) = delete;

		/* scheme constructor */
		vim(const std::string& path, const ucg::colorscheme& scheme)
		: _highlights{}, _path{path} {

			using cs = ucg::colorscheme;


			add("Normal",          cs::G7(scheme), cs::G1(scheme), "NONE");
			add("NormalNC",        cs::G7(scheme), cs::G1(scheme), "NONE");
			add("Title",           cs::C1(scheme), "NONE",         "NONE");
			add("Directory",       cs::C1(scheme), "NONE",         "NONE");
			add("ColorColumn",     "NONE",         cs::G2(scheme), "NONE");

			add("Pmenu",           "Normal");
			add("PmenuKind",       "Pmenu");
			add("PmenuExtra",      "Pmenu");
			add("PmenuSel",        cs::B2(scheme), "NONE",         "NONE");
			add("PmenuKindSel",    "PmenuSel");
			add("PmenuExtraSel",   "PmenuSel");
			add("PmenuSbar",       "NONE",         cs::G6(scheme), "NONE");
			add("PmenuThumb",      cs::G2(scheme), "NONE",         "NONE");

			add("WildMenu",        cs::B2(scheme), "NONE",         "NONE");

			add("DiffAdd",         cs::A1(scheme), "NONE",         "NONE");
			add("DiffChange",      cs::B1(scheme), "NONE",         "NONE");
			add("DiffDelete",      cs::C1(scheme), "NONE",         "NONE");
			add("DiffText",        "Normal");

			add("SpellBad",        cs::A1(scheme), "NONE",         "UNDERLINE");
			add("SpellCap",        "SpellBad");
			add("SpellLocal",      "SpellBad");
			add("SpellRare",       "SpellBad");

			add("Cursor",          "NONE",         "NONE",         "NONE");
			add("lCursor",         "NONE",         "NONE",         "NONE");
			add("CursorIM",        "NONE",         "NONE",         "NONE");

			add("CursorLine",      "NONE",         cs::G2(scheme), "bold");
			add("CursorLineNr",    cs::A2(scheme), "NONE",         "NONE");
			add("CursorColumn",    "CursorLine");

			add("TermCursor",      "NONE",         "NONE",         "NONE");
			add("TermCursorNC",    "NONE",         "NONE",         "NONE");
			add("CursorLineFold",  "NONE",         "NONE",         "NONE");
			add("CursorLineSign",  "NONE",         "NONE",         "NONE");

			add("NonText",         cs::G3(scheme), "NONE",         "NONE");
			add("Conceal",         "NonText");
			add("Whitespace",      cs::G2(scheme), "NONE",         "NONE");
			add("EndOfBuffer",     "NonText");
			add("SpecialKey",      cs::A2(scheme), "NONE",         "NONE");

			add("Search",          "NONE",         cs::G6(scheme), "NONE");
			add("IncSearch",       "Search");
			add("CurSearch",       "Search");
			add("Substitute",      "Search");

			add("Folded",          cs::B2(scheme), "NONE",         "NONE");
			add("FoldColumn",      "Folded");

			add("SignColumn",      cs::A2(scheme), "NONE",         "NONE");
			add("LineNr",          cs::G2(scheme), "NONE",         "NONE");
			add("LineNrAbove",     "LineNr");
			add("LineNrBelow",     "LineNr");

			add("MatchParen",      cs::G6(scheme), cs::G2(scheme), "NONE");

			add("ModeMsg",         cs::G5(scheme), "NONE",         "NONE");
			add("MsgArea",         cs::G5(scheme), "NONE",         "NONE");
			add("MsgSeparator",    "Normal");
			add("MoreMsg",         "Normal");
			add("ErrorMsg",        cs::A1(scheme), "NONE",         "NONE");
			add("WarningMsg",      cs::B1(scheme), "NONE",         "NONE");
			add("Question",        cs::C1(scheme), "NONE",         "NONE");

			add("NormalFloat",     "Normal");
			add("FloatBorder",     cs::G2(scheme), "NONE",         "NONE");
			add("FloatTitle",      "Title");

			add("WinSeparator",    "NonText");

			add("QuickFixLine",    "Normal");

			add("TabLine",         cs::G5(scheme), cs::G2(scheme), "NONE");
			add("TabLineSel",      cs::G6(scheme), cs::G3(scheme), "NONE");
			add("TabLineFill",     cs::G5(scheme), cs::G2(scheme), "NONE");

			add("StatusLine",      cs::G5(scheme), cs::G2(scheme), "NONE");
			add("StatusLineNC",    "Normal");

			add("Visual",          "MatchParen");
			add("VisualNOS",       "NONE",        "NONE",          "NONE");
			add("WinBar",          "NONE",        "NONE",          "NONE");
			add("WinBarNC",        "NONE",        "NONE",          "NONE");
			add("Menu",            "NONE",        "NONE",          "NONE");
			add("ScrollBar",       "NONE",        "NONE",          "NONE");
			add("Tooltip",         "NONE",        "NONE",          "NONE");


			// -- syntax highlighting -----------------------------------------

			add("Identifier",      cs::G7(scheme), "NONE",         "NONE");

			add("Type",            cs::B2(scheme), "NONE",         "italic");

			add("String",          cs::C2(scheme), "NONE",         "italic");
			add("Character",       "String");

			add("Special",         cs::C1(scheme), "NONE",         "italic");
			add("SpecialChar",     "Special");

			add("Number",          cs::C2(scheme), "NONE",         "NONE");
			add("Constant",        cs::C2(scheme), "NONE",         "NONE");

			add("Comment",         "NonText");
			add("Keyword",         cs::A2(scheme), "NONE",         "NONE");
			add("Structure",       cs::A1(scheme), "NONE",         "NONE");
			add("StorageClass",    cs::A2(scheme), "NONE",         "NONE");
			add("Typedef",         "Keyword");

			add("Conditional",     "Keyword");
			add("Repeat",          "Keyword");
			add("Statement",       cs::B2(scheme), "NONE",         "NONE");
			add("Label",           cs::B2(scheme), "NONE",         "NONE");

			add("Operator",        cs::B1(scheme), "NONE",         "NONE");
			add("Delimiter",       "Operator");

			add("Function",        cs::B1(scheme), "NONE",         "bold");

			add("Exception",       "Keyword");

			add("Define",          cs::A1(scheme), "NONE",         "NONE");
			add("Preproc",         "Define");
			add("Precondit",       "Define");
			add("Include",         "Define");

			add("Todo",            cs::A1(scheme), "NONE",         "NONE");


			// -- treesitter --------------------------------------------------

			add("None",            "Identifier");
			add("@text.literal",   "Comment");
			add("@text.reference", "Identifier");
			add("@text.title",     "Title");
			add("@text.underline", "Identifier");
			add("@text.uri",	   "Identifier");
			add("@text.todo",      "Todo");

			add("@comment",        "Comment");

			add("@punctuation",    cs::G4(scheme), "NONE", 	   "NONE");
			add("@punctuation.delimiter", "@punctuation");
			add("@punctuation.bracket",  "@punctuation");
			add("@punctuation.special",  "@punctuation");

			add("@constant",        "Constant");
			add("@constant.builtin", "Special");
			add("@constant.macro",   "Define");

			add("@define",          "Define");
			add("@macro",           "Define");

			add("@string",          "String");
			add("@string.escape",   "SpecialChar");
			add("@string.special",  "SpecialChar");

			add("character",        "Character");
			add("character.special","SpecialChar");

			add("@number",          "Number");
			add("@boolean",         "Number");
			add("@float",           "Number");

			add("@function",        "Function");
			add("@function.builtin","Special");
			add("@function.macro",  "Define");
			add("@constructor",     "Special");
			add("@method",          "Function");

			add("@parameter",       "Identifier");
			add("@field",           "Identifier");
			add("@property",        "Identifier");
			add("@variable",        "Identifier");

			add("@conditional",      "Conditional");
			add("@repeat",           "Repeat");
			add("@label",            "Label");

			add("@operator",         "Operator");
			add("@keyword",          "Keyword");
			add("@keyword.operator", "Operator");
			add("@exception",        "Exception");

			add("@module",           "Type");
			add("@type",             "Type");
			add("@type.definition",  "Type");
			add("@structure",        "Structure");
			add("@namespace",        "Type");

			add("@storageclass",     "StorageClass");

			add("@include",          "Include");
			add("@preproc",          "Preproc");

			add("@debug",            "Todo");

			add("@tag",              "Identifier");


			add("@commment.documentation", "Comment");

			add("@type.qualifier",   "StorageClass");


			add("@string.documetation", "String");
			add("@string.regex",        "String");

			add("@parameter.builtin",   "Keyword");
			add("@parameter.coroutine", "Keyword");
			add("@keyword.function",    "Keyword");

			// auto, int, float, etc...
			add("@type.builtin",        "Type");

			// this
			add("@variable.builtin",    "Keyword");

			add("@namespace.builtin",   "Type");

		}

		vim(const vim&) = delete;
		~vim(void) = default;


		void generate(void) {


			std::string content;
			content.reserve(4096);

			content.assign(
					"\" only support full color terminal\n"\
					"if !has('termguicolors')\n"\
					"\tfinish\n"\
					"endif\n"\
					"\" set background\n"\
					"set background=dark\n"\
					"\" reset existing highlighting\n"\
					"hi clear\n"\
					"\" reset existing syntax highlighting\n"\
					"if exists(\"syntax_on\")\n"\
					"\tsyntax reset\n"\
					"endif\n"\
					"\" set scheme name\n"\
					"let g:colors_name = 'ucg'\n\n"
			);



			for (auto& hi : _highlights) {
				content.append("hi! ");
				content.append(hi.first);
				content.append(" guifg=");
				content.append(hi.second.foreground());
				content.append(" guibg=");
				content.append(hi.second.background());
				content.append(" gui=");
				content.append(hi.second.style());
				content.append("\n");

			}


			//std::cout << content << std::endl;

			std::ofstream file{_path};
			if (file.is_open()) {
				file << content;
				file.close();
			}


		}

	private:

		std::unordered_map<std::string_view,
							highlight> _highlights;

		std::string _path;

};





#endif
