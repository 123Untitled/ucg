/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf --output-file=gp_hash.hpp --delimiters=, --language=C++ --hash-function-name=hash --lookup-function-name=lookup --class-name=perfect_hash tmp.keys  */
/* Computed positions: -k'1,3,9,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif


#define TOTAL_KEYWORDS 227
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 26
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 606
/* maximum key range = 600, duplicates = 0 */

class perfect_hash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const char *lookup (const char *str, unsigned int len);
};

inline unsigned int
perfect_hash::hash (register const char *str, register unsigned int len)
{
  static unsigned short asso_values[] =
    {
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607,  80, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607,   0, 607, 607,  25,   5, 155,
       35, 607, 607, 195, 607,   5, 240, 185,  50, 245,
      140,  65,  10, 125,  75, 607,  25, 215, 607, 607,
      607, 607, 607, 607, 607, 607, 607,   5, 150,   4,
      115,   0, 245,  30,  90,   0, 607, 250,  75, 150,
       45,  30,   5, 607,  80, 110,  10,  20, 607,   5,
      220, 130, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607, 607, 607, 607, 607,
      607, 607, 607, 607, 607, 607
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
      case 7:
      case 6:
      case 5:
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

const char *
perfect_hash::lookup (register const char *str, register unsigned int len)
{
  static const char * wordlist[] =
    {
      "", "", "", "", "", "", "",
      "@define",
      "",
      "@text.uri",
      "", "", "", "",
      "@variable",
      "", "",
      "@repeat",
      "",
      "@namespace",
      "",
      "@text.title",
      "", "",
      "@string.escape",
      "@parameter.coroutine",
      "", "", "", "", "",
      "Repeat",
      "", "",
      "DiagnosticHint",
      "",
      "@debug",
      "@module",
      "DiagnosticSignHint",
      "@tag",
      "",
      "@macro",
      "DiagnosticFloatingHint",
      "DiagnosticUnderlineHint",
      "",
      "DiagnosticVirtualTextHint",
      "",
      "@string",
      "@comment",
      "", "", "", "",
      "@include",
      "DiagnosticInfo",
      "", "", "",
      "DiagnosticSignInfo",
      "@constant",
      "@punctuation.bracket",
      "",
      "DiagnosticFloatingInfo",
      "DiagnosticUnderlineInfo",
      "",
      "DiagnosticVirtualTextInfo",
      "",
      "@variable.builtin",
      "@parameter.builtin",
      "DiagnosticWarn",
      "", "",
      "@namespace.builtin",
      "DiagnosticSignWarn",
      "", "", "",
      "DiagnosticFloatingWarn",
      "DiagnosticUnderlineWarn",
      "",
      "DiagnosticVirtualTextWarn",
      "", "",
      "@boolean",
      "Type",
      "@constant.macro",
      "@label",
      "@punctuation",
      "Constant",
      "",
      "Title",
      "@float",
      "@preproc",
      "NvimTreeCursorLine",
      "",
      "@parameter",
      "", "",
      "@text.literal",
      "None",
      "@structure",
      "",
      "@constant.builtin",
      "NvimTreeFolderName",
      "",
      "DiagnosticError",
      "",
      "@number",
      "@commment.documentation",
      "DiagnosticSignError",
      "@function.macro",
      "",
      "NonText",
      "DiagnosticFloatingError",
      "DiagnosticUnderlineError",
      "",
      "DiagnosticVirtualTextError",
      "Tooltip",
      "Question",
      "@function",
      "",
      "@field",
      "@method",
      "@keyword",
      "",
      "@punctuation.special",
      "@constructor",
      "@function.builtin",
      "",
      "NvimTreeNormalFloat",
      "@text.underline",
      "",
      "@punctuation.delimiter",
      "Function",
      "SpellRare",
      "@type",
      "",
      "@storageclass",
      "SpellCap",
      "",
      "NvimTreeCursorColumn",
      "NvimTreeNormalNC",
      "@keyword.function",
      "",
      "NvimTreeClosedFolderIcon",
      "DiagnosticDeprecated",
      "NvimTreeFileIcon",
      "@conditional",
      "NvimTreeFolderIcon",
      "",
      "FloatTitle",
      "Preproc",
      "Conceal",
      "NvimTreeFolderArrowOpen",
      "",
      "@text.todo",
      "", "",
      "NvimTreeRootFolder",
      "Statement",
      "CursorLine",
      "DiagnosticUnnecessary",
      "",
      "NormalNC",
      "TelescopeTitle",
      "Pmenu",
      "", "", "",
      "Precondit",
      "TelescopePromptTitle",
      "", "", "",
      "@operator",
      "NvimTreeCursorLineNr",
      "ColorColumn",
      "@keyword.operator",
      "", "",
      "@character",
      "NormalFloat",
      "lCursor",
      "@character.special",
      "",
      "StatusLine",
      "", "",
      "@type.builtin",
      "NvimTreeNormal",
      "@string.documetation",
      "Cursor",
      "Comment",
      "NvimTreeStatusLine",
      "NvimTreeSpecialFile",
      "", "",
      "TelescopeMatching",
      "",
      "Character",
      "NvimTreeSymlinkFolderName",
      "Conditional",
      "", "", "", "",
      "Include",
      "Special",
      "NvimTreePopup",
      "CursorLineSign",
      "@string.special",
      "Normal",
      "StatusLineNC",
      "TelescopeSelection",
      "Structure",
      "SpellLocal",
      "Visual",
      "", "", "",
      "NvimTreeStatusLineNC",
      "NvimTreeExecFile",
      "TermCursorNC",
      "PmenuSel",
      "Todo",
      "NvimTreeFolderArrowClosed",
      "Search",
      "MsgArea",
      "NvimTreeEmptyFolderName",
      "Whitespace",
      "@type.qualifier",
      "Folded",
      "TabLine",
      "", "",
      "PmenuExtra",
      "",
      "CursorColumn",
      "NvimTreeSignColumn",
      "NvimTreeSymlinkIcon",
      "TelescopeNormal",
      "String",
      "CursorLineNr",
      "", "",
      "TelescopeBorder",
      "TelescopePromptNormal",
      "",
      "SpellBad",
      "Delimiter",
      "MatchParen",
      "",
      "TelescopePromptCounter",
      "",
      "Menu",
      "NvimTreeModifiedFile",
      "Define",
      "Keyword",
      "Exception",
      "",
      "@text.reference",
      "",
      "NvimTreeImageFile",
      "", "",
      "SpecialKey",
      "", "",
      "DiffText",
      "",
      "DiffDelete",
      "FloatBorder",
      "DiagnosticOk",
      "ErrorMsg",
      "",
      "TermCursor",
      "DiagnosticSignOk",
      "", "",
      "CursorLineFold",
      "DiagnosticFloatingOk",
      "DiagnosticUnderlineOk",
      "",
      "DiagnosticVirtualTextOk",
      "",
      "Identifier",
      "Number",
      "", "", "",
      "DiffChange",
      "", "",
      "WinBarNC",
      "CurSearch",
      "Substitute",
      "", "",
      "CursorIM",
      "", "", "",
      "MoreMsg",
      "",
      "NvimTreeEndOfBuffer",
      "@exception",
      "SpecialChar",
      "",
      "PmenuExtraSel",
      "PmenuSbar",
      "TabLineSel",
      "TabLineFill",
      "MsgSeparator",
      "NvimTreeOpenedFile",
      "",
      "FoldColumn",
      "",
      "QuickFixLine",
      "WildMenu",
      "NvimTreeOpenedFolderName",
      "", "", "",
      "@string.regex",
      "", "",
      "LineNrAbove",
      "", "", "", "", "",
      "Typedef",
      "Operator",
      "", "", "",
      "ModeMsg",
      "", "", "", "",
      "PmenuKindSel",
      "", "",
      "NvimTreeIndentMarker",
      "WinBar",
      "", "",
      "@property",
      "", "",
      "StorageClass",
      "",
      "Directory",
      "", "",
      "WinSeparator",
      "", "",
      "SignColumn",
      "",
      "NvimTreeOpenedFileIcon",
      "",
      "NvimTreeOpenedFolderIcon",
      "NvimTreeWinSeparator",
      "", "", "", "", "",
      "LineNr",
      "DiffAdd",
      "",
      "ScrollBar",
      "",
      "LineNrBelow",
      "", "",
      "PmenuKind",
      "", "", "", "",
      "NvimTreeLineNr",
      "", "", "",
      "IncSearch",
      "", "", "", "", "",
      "VisualNOS",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "",
      "@type.definition",
      "", "", "",
      "NvimTreeSymlink",
      "", "", "", "",
      "WarningMsg",
      "", "", "", "",
      "PmenuThumb",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "",
      "Label",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "EndOfBuffer"
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key];

          if (*str == *s && !strcmp (str + 1, s + 1))
            return s;
        }
    }
  return 0;
}
