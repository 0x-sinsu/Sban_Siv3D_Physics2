#pragma once

#include <cstdint>

// アルファベット
constexpr uint32_t UNICODE_ALPHA_START = 0x0041;
constexpr uint32_t UNICODE_ALPHA_END = 0x005A;
constexpr uint32_t UNICODE_ALPHA_LOWER_START = 0x0061;
constexpr uint32_t UNICODE_ALPHA_LOWER_END = 0x007A;

// 数字
constexpr uint32_t UNICODE_NUMBER_START = 0x0030;
constexpr uint32_t UNICODE_NUMBER_END = 0x0039;

// プリント可能な ASCII 文字
constexpr uint32_t PRINTABLE_ASCII_START = 0x0020; // ' '
constexpr uint32_t PRINTABLE_ASCII_END = 0x007E; // '~'

// 記号
constexpr uint32_t SYMBOL_START = 0x0021; // '!'
constexpr uint32_t SYMBOL_END = 0x002F; // '/'
constexpr uint32_t SYMBOL_EXTENDED_START = 0x003A; // ':'
constexpr uint32_t SYMBOL_EXTENDED_END = 0x0040; // '@'
constexpr uint32_t SYMBOL_MISC_START = 0x005B; // '['
constexpr uint32_t SYMBOL_MISC_END = 0x0060; // '`'
constexpr uint32_t SYMBOL_OTHER_START = 0x007B; // '{'
constexpr uint32_t SYMBOL_OTHER_END = 0x007E; // '~'

// その他の記号や特殊文字
constexpr uint32_t MISC_SYMBOL_START = 0x00A1;
constexpr uint32_t MISC_SYMBOL_END = 0x00BF;
constexpr uint32_t GENERAL_PUNCTUATION_START = 0x2000;
constexpr uint32_t GENERAL_PUNCTUATION_END = 0x206F;
constexpr uint32_t CJK_SYMBOLS_PUNCTUATION_START = 0x3000;
constexpr uint32_t CJK_SYMBOLS_PUNCTUATION_END = 0x303F;
constexpr uint32_t HALFWIDTH_FULLWIDTH_FORMS_START = 0xFF00;
constexpr uint32_t HALFWIDTH_FULLWIDTH_FORMS_END = 0xFFEF;


// 平仮名
constexpr uint32_t HIRAGANA_START = 0x3040;
constexpr uint32_t HIRAGANA_END = 0x309F;

// 片仮名
constexpr uint32_t KATAKANA_START = 0x30A0;
constexpr uint32_t KATAKANA_END = 0x30FF;

// 半角片仮名
constexpr uint32_t HALF_WIDTH_KATAKANA_START = 0xFF65;
constexpr uint32_t HALF_WIDTH_KATAKANA_END = 0xFF9F;

// 片仮名拡張
constexpr uint32_t KATAKANA_PHONETIC_EXTENSIONS_START = 0x31F0;
constexpr uint32_t KATAKANA_PHONETIC_EXTENSIONS_END = 0x31FF;

// 各種漢字
constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_START = 0x4E00;
constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_END = 0x9FFF;

constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A_START = 0x3400;
constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A_END = 0x4DBF;

constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B_START = 0x20000;
constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B_END = 0x2A6DF;

constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C_START = 0x2A700;
constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C_END = 0x2B73F;

constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D_START = 0x2B740;
constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D_END = 0x2B81F;

constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E_START = 0x2B820;
constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E_END = 0x2CEAF;

constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F_START = 0x2CEB0;
constexpr uint32_t KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F_END = 0x2EBEF;


// Function to check if a character is within a specific range
inline bool is_in_range(uint32_t code_point, uint32_t start, uint32_t end)
{
	return code_point >= start && code_point <= end;
}

// Specific functions to check if a character belongs to a category
inline bool is_alphabet_upper_case(uint32_t code_point)
{
	return is_in_range(code_point, UNICODE_ALPHA_START, UNICODE_ALPHA_END);
}

inline bool is_alphabet_lower_case(uint32_t code_point)
{
	return is_in_range(code_point, UNICODE_ALPHA_LOWER_START,
	                   UNICODE_ALPHA_LOWER_END);
}

inline bool is_number(uint32_t code_point)
{
	return is_in_range(code_point, UNICODE_NUMBER_START, UNICODE_NUMBER_END);
}

inline bool is_hiragana(uint32_t code_point)
{
	return is_in_range(code_point, HIRAGANA_START, HIRAGANA_END);
}

inline bool is_katakana(uint32_t code_point)
{
	return is_in_range(code_point, KATAKANA_START, KATAKANA_END);
}

inline bool is_half_width_katakana(uint32_t code_point)
{
	return is_in_range(code_point, HALF_WIDTH_KATAKANA_START,
	                   HALF_WIDTH_KATAKANA_END);
}

inline bool is_katakana_phonetic_extensions(uint32_t code_point)
{
	return is_in_range(code_point, KATAKANA_PHONETIC_EXTENSIONS_START,
	                   KATAKANA_PHONETIC_EXTENSIONS_END);
}

inline bool is_kanji(uint32_t code_point)
{
	return is_in_range(code_point, KANJI_CJK_UNIFIED_IDEOGRAPHS_START,
	                   KANJI_CJK_UNIFIED_IDEOGRAPHS_END) ||
		is_in_range(code_point, KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A_START,
		            KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A_END) ||
		is_in_range(code_point, KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B_START,
		            KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B_END) ||
		is_in_range(code_point, KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C_START,
		            KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C_END) ||
		is_in_range(code_point, KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D_START,
		            KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D_END) ||
		is_in_range(code_point, KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E_START,
		            KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E_END) ||
		is_in_range(code_point, KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F_START,
		            KANJI_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F_END);
}

bool is_opt_char(
	char32_t ch, const std::unordered_map<char32_t, double>& option_ratios)
{
	return option_ratios.contains(ch);
}
