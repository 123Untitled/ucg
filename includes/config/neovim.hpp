#ifndef UCG_NEOVIM_HPP
#define UCG_NEOVIM_HPP

#include <string>
#include <fstream>
#include <unordered_map>

#include "hex.hpp"
#include "colorscheme.hpp"


// -- C O N F I G  N A M E S P A C E ------------------------------------------

namespace config {


	// -- N E O V I M ---------------------------------------------------------

	class neovim final {


		private:

			// -- private class -----------------------------------------------

			class highlight final {

				public:

					// -- public types ----------------------------------------

					/* self type */
					using self = config::neovim::highlight;


					// -- public lifecycle ------------------------------------


					/* default constructor */
					inline constexpr highlight(void)
					: _foreground{}, _background{}, _style{} {}

					inline highlight(const std::string& fg,
									 const std::string& bg,
									 const std::string_view& style)
					: _foreground{fg}, _background{bg}, _style{style} {}

					inline highlight(const color::hex& fg,
									 const color::hex& bg,
									 const std::string_view& style)
					: _foreground{fg}, _background{bg}, _style{style} {}

					inline highlight(const std::string& fg,
									 const color::hex& bg,
									 const std::string_view& style)
					: _foreground{fg}, _background{bg}, _style{style} {}

					inline highlight(const color::hex& fg,
									 const std::string& bg,
									 const std::string_view& style)
					: _foreground{fg}, _background{bg}, _style{style} {}


					/* copy constructor */
					inline constexpr highlight(const self&) = default;

					/* move constructor */
					inline constexpr highlight(self&& other) noexcept = default;


					// -- public assignment operators -----------------------------

					/* copy assignment operator */
					inline auto operator=(const self& other) -> self& = default;

					/* move assignment operator */
					inline auto operator=(self&& other) noexcept -> self& = default;


					// -- public accessors ------------------------------------

					/* foreground */
					inline auto foreground(void) const -> const std::string& {
						return _foreground; }

					/* background */
					inline auto background(void) const -> const std::string& {
						return _background; }

					/* style */
					inline auto style(void) const -> const std::string_view& {
						return _style; }

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

			template <decltype(sizeof(0)) N>
			inline auto key(const char (&group)[N]) const -> const highlight& {
				return _highlights.at(group);
			}

			template <decltype(sizeof(0)) N>
			inline auto link(const char (&group)[N]) -> highlight& {
				return _highlights[group];
			}


		public:

			/* deleted default constructor */
			neovim(void) = delete;

			/* scheme constructor */
			neovim(const std::string& path, const ucg::colorscheme& scheme)
			: _highlights{}, _path{path} {

				using cs = ucg::colorscheme;


				// normal text
				add("Normal",          cs::G7(scheme), cs::G1(scheme), "NONE");

				// normal text in non-current windows.
				add("NormalNC",        cs::G7(scheme), cs::G1(scheme), "NONE");

				// titles for output from ":set all", ":autocmd" etc.
				add("Title",           cs::C1(scheme), "NONE",         "NONE");

				// directory names (and other special names in listings).
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


				// -- treesitter ----------------------------------------------

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

				add("@character",        "Character");
				add("@character.special","SpecialChar");

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


				// -- diagnostics ---------------------------------------------

				// ok
				add("DiagnosticOk", "MsgArea");
				// info
				add("DiagnosticInfo", "ModeMsg");
				// warning
				add("DiagnosticWarn", "WarningMsg");
				// error
				add("DiagnosticError", "ErrorMsg");
				// hint
				add("DiagnosticHint", "MsgArea");

				// link
				link("DiagnosticUnderlineOk")      = key("DiagnosticOk");
				link("DiagnosticUnderlineInfo")    = key("DiagnosticInfo");
				link("DiagnosticUnderlineWarn")    = key("DiagnosticWarn");
				link("DiagnosticUnderlineError")   = key("DiagnosticError");
				link("DiagnosticUnderlineHint")    = key("DiagnosticHint");

				link("DiagnosticVirtualTextOk")    = key("DiagnosticOk");
				link("DiagnosticVirtualTextInfo")  = key("DiagnosticInfo");
				link("DiagnosticVirtualTextWarn")  = key("DiagnosticWarn");
				link("DiagnosticVirtualTextError") = key("DiagnosticError");
				link("DiagnosticVirtualTextHint")  = key("DiagnosticHint");

				add("DiagnosticFloatingOk", "DiagnosticOk");
				add("DiagnosticFloatingInfo", "DiagnosticInfo");
				add("DiagnosticFloatingWarn", "DiagnosticWarn");
				add("DiagnosticFloatingError", "DiagnosticError");
				add("DiagnosticFloatingHint", "DiagnosticHint");

				add("DiagnosticSignOk", "DiagnosticOk");
				add("DiagnosticSignInfo", "DiagnosticInfo");
				add("DiagnosticSignWarn", "DiagnosticWarn");
				add("DiagnosticSignError", "DiagnosticError");
				add("DiagnosticSignHint", "DiagnosticHint");

				add("DiagnosticDeprecated", "Normal");
				add("DiagnosticUnnecessary", "Normal");


				// -- telescope -----------------------------------------------

				// main title
				link("TelescopeTitle") = key("Title");

				// border
				link("TelescopeBorder") = key("FloatBorder");

				// normal text
				add("TelescopeNormal", cs::G3(scheme), "NONE", "NONE");

				// prompt title
				link("TelescopePromptTitle") = key("Comment");

				// prompt normal text
				add("TelescopePromptNormal", cs::G4(scheme), "NONE", "NONE");

				// selected item
				add("TelescopeSelection", cs::B2(scheme), "NONE", "NONE");

				// matched characters
				add("TelescopeMatching", cs::G5(scheme), "NONE", "NONE");

				// ???
				add("TelescopePromptCounter", cs::A2(scheme), "NONE", "NONE");


				// nvim-tree

				// normal text
				link("NvimTreeNormal") = key("Comment");
				// normal text in floating windows.
				link("NvimTreeNormalFloat") = key("NormalFloat");
				// normal text in non-current windows.
				link("NvimTreeNormalNC") = key("Comment");

				link("NvimTreeLineNr") = key("LineNr");

				link("NvimTreeWinSeparator") = key("WinSeparator");
				link("NvimTreeEndOfBuffer") = key("EndOfBuffer");
				link("NvimTreePopup") = key("NormalFloat");
				link("NvimTreeSignColumn") = key("SignColumn");
				link("NvimTreeCursorColumn") = key("CursorColumn");
				link("NvimTreeCursorLine") = key("CursorLine");
				link("NvimTreeCursorLineNr") = key("CursorLineNr");
				link("NvimTreeStatusLine") = key("StatusLine");
				link("NvimTreeStatusLineNC") = key("StatusLineNC");

				link("NvimTreeSymlink") = key("Special");
				link("NvimTreeExecFile") = key("Type");
				add("NvimTreeOpenedFile", cs::G7(scheme), "NONE",         "NONE");
				link("NvimTreeModifiedFile") = key("DiffChange");
				link("NvimTreeSpecialFile") = key("String");
				link("NvimTreeImageFile") = key("Constant");

				link("NvimTreeFolderName")        = key("Directory");
				link("NvimTreeEmptyFolderName")   = key("Comment");
				link("NvimTreeOpenedFolderName")  = key("Type");
				link("NvimTreeSymlinkFolderName") = key("NvimTreeSymlink");
				link("NvimTreeRootFolder")        = key("NvimTreeFolderName");
 
				link("NvimTreeFileIcon") = key("NvimTreeNormal");
				link("NvimTreeOpenedFileIcon") = key("NvimTreeOpenedFile");
				link("NvimTreeSymlinkIcon") = key("NvimTreeSymlink");
				link("NvimTreeFolderIcon") = key("NvimTreeFolderName");
				link("NvimTreeOpenedFolderIcon") = key("NvimTreeOpenedFolderName");
				link("NvimTreeClosedFolderIcon") = key("NvimTreeFolderName");
				link("NvimTreeFolderArrowClosed") = key("NvimTreeFolderName");
				link("NvimTreeFolderArrowOpen") = key("NvimTreeOpenedFolderName");


				link("NvimTreeIndentMarker") = key("NvimTreeNormal");
// 
// 
//Clipboard:  
//    NvimTreeCopiedHL            SpellRare
//    NvimTreeCutHL               SpellBad
// 
//Bookmark Icon:  
//    NvimTreeBookmark
// 
//Bookmark Highlight:  
//    NvimTreeBookmarkHL          SpellLocal
// 
//Picker:  
//    NvimTreeWindowPicker
// 
//Live Filter:  
//    NvimTreeLiveFilterPrefix
//    NvimTreeLiveFilterValue
// 
//Git Icon:  
//    NvimTreeGitDirty
//    NvimTreeGitStaged
//    NvimTreeGitMerge
//    NvimTreeGitRenamed
//    NvimTreeGitNew
//    NvimTreeGitDeleted
//    NvimTreeGitIgnored                  Comment
// 
//Git File Text:  
//    NvimTreeFileDirty                   NvimTreeGitDirty
//    NvimTreeFileStaged                  NvimTreeGitStaged
//    NvimTreeFileMerge                   NvimTreeGitMerge
//    NvimTreeFileRenamed                 NvimTreeGitRenamed
//    NvimTreeFileNew                     NvimTreeGitNew
//    NvimTreeFileDeleted                 NvimTreeGitDeleted
//    NvimTreeFileIgnored                 NvimTreeGitIgnored
// 
//Git Folder Text:  
//    NvimTreeFolderDirty                 NvimTreeFileDirty
//    NvimTreeFolderStaged                NvimTreeFileStaged
//    NvimTreeFolderMerge                 NvimTreeFileMerge
//    NvimTreeFolderRenamed               NvimTreeFileRenamed
//    NvimTreeFolderNew                   NvimTreeFileNew
//    NvimTreeFolderDeleted               NvimTreeFileDeleted
//    NvimTreeFolderIgnored               NvimTreeFileIgnored
// 
//Diagnostics Icon:  
//    NvimTreeLspDiagnosticsError             DiagnosticError
//    NvimTreeLspDiagnosticsWarning           DiagnosticWarn
//    NvimTreeLspDiagnosticsInformation       DiagnosticInfo
//    NvimTreeLspDiagnosticsHint              DiagnosticHint
// 
//Diagnostics File Text:  
//    NvimTreeLspDiagnosticsErrorText         NvimTreeLspDiagnosticsError
//    NvimTreeLspDiagnosticsWarningText       NvimTreeLspDiagnosticsWarning
//    NvimTreeLspDiagnosticsInfoText          NvimTreeLspDiagnosticsInformation
//    NvimTreeLspDiagnosticsHintText          NvimTreeLspDiagnosticsHint
// 
//Diagnostics Folder Text:  
//    NvimTreeLspDiagnosticsErrorFolderText   NvimTreeLspDiagnosticsErrorText
//    NvimTreeLspDiagnosticsWarningFolderText NvimTreeLspDiagnosticsWarningText
//    NvimTreeLspDiagnosticsInfoFolderText    NvimTreeLspDiagnosticsInfoText
//    NvimTreeLspDiagnosticsHintFolderText    NvimTreeLspDiagnosticsHintText



			}

			neovim(const neovim&) = delete;
			~neovim(void) = default;


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

}

#endif // UCG_NEOVIM_HPP
