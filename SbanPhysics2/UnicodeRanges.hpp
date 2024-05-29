#pragma once

#include <cstdint>

// Hiragana Unicode range
constexpr uint32_t HIRAGANA_START = 0x3040;
constexpr uint32_t HIRAGANA_END = 0x309F;

// Katakana Unicode range
constexpr uint32_t KATAKANA_START = 0x30A0;
constexpr uint32_t KATAKANA_END = 0x30FF;

// Half-width Katakana Unicode range
constexpr uint32_t HALF_WIDTH_KATAKANA_START = 0xFF65;
constexpr uint32_t HALF_WIDTH_KATAKANA_END = 0xFF9F;

// Katakana Phonetic Extensions Unicode range
constexpr uint32_t KATAKANA_PHONETIC_EXTENSIONS_START = 0x31F0;
constexpr uint32_t KATAKANA_PHONETIC_EXTENSIONS_END = 0x31FF;

// CJK Unified Ideographs Unicode range
constexpr uint32_t CJK_UNIFIED_IDEOGRAPHS_START = 0x4E00;
constexpr uint32_t CJK_UNIFIED_IDEOGRAPHS_END = 0x9FFF;

// CJK Extension A Unicode range
constexpr uint32_t CJK_EXTENSION_A_START = 0x3400;
constexpr uint32_t CJK_EXTENSION_A_END = 0x4DBF;

// CJK Extension B Unicode range
constexpr uint32_t CJK_EXTENSION_B_START = 0x20000;
constexpr uint32_t CJK_EXTENSION_B_END = 0x2A6DF;

// CJK Extension C Unicode range
constexpr uint32_t CJK_EXTENSION_C_START = 0x2A700;
constexpr uint32_t CJK_EXTENSION_C_END = 0x2B73F;

// CJK Extension D Unicode range
constexpr uint32_t CJK_EXTENSION_D_START = 0x2B740;
constexpr uint32_t CJK_EXTENSION_D_END = 0x2B81F;

// CJK Extension E Unicode range
constexpr uint32_t CJK_EXTENSION_E_START = 0x2B820;
constexpr uint32_t CJK_EXTENSION_E_END = 0x2CEAF;

// CJK Extension F Unicode range
constexpr uint32_t CJK_EXTENSION_F_START = 0x2CEB0;
constexpr uint32_t CJK_EXTENSION_F_END = 0x2EBEF;

// Japanese Punctuation Unicode range
constexpr uint32_t JAPANESE_PUNCTUATION_START = 0x3000;
constexpr uint32_t JAPANESE_PUNCTUATION_END = 0x303F;

// Function to check if a character is within a specific range
inline bool isInRange(uint32_t codePoint, uint32_t start, uint32_t end) {
  return codePoint >= start && codePoint <= end;
}

// Specific functions to check if a character belongs to a category
inline bool isHiragana(uint32_t codePoint) {
  return isInRange(codePoint, HIRAGANA_START, HIRAGANA_END);
}

inline bool isKatakana(uint32_t codePoint) {
  return isInRange(codePoint, KATAKANA_START, KATAKANA_END);
}

inline bool isHalfWidthKatakana(uint32_t codePoint) {
  return isInRange(codePoint, HALF_WIDTH_KATAKANA_START,
                   HALF_WIDTH_KATAKANA_END);
}

inline bool isKatakanaPhoneticExtensions(uint32_t codePoint) {
  return isInRange(codePoint, KATAKANA_PHONETIC_EXTENSIONS_START,
                   KATAKANA_PHONETIC_EXTENSIONS_END);
}

inline bool isCJKUnifiedIdeographs(uint32_t codePoint) {
  return isInRange(codePoint, CJK_UNIFIED_IDEOGRAPHS_START,
                   CJK_UNIFIED_IDEOGRAPHS_END);
}

inline bool isCJKExtensionA(uint32_t codePoint) {
  return isInRange(codePoint, CJK_EXTENSION_A_START, CJK_EXTENSION_A_END);
}

inline bool isCJKExtensionB(uint32_t codePoint) {
  return isInRange(codePoint, CJK_EXTENSION_B_START, CJK_EXTENSION_B_END);
}

inline bool isCJKExtensionC(uint32_t codePoint) {
  return isInRange(codePoint, CJK_EXTENSION_C_START, CJK_EXTENSION_C_END);
}

inline bool isCJKExtensionD(uint32_t codePoint) {
  return isInRange(codePoint, CJK_EXTENSION_D_START, CJK_EXTENSION_D_END);
}

inline bool isCJKExtensionE(uint32_t codePoint) {
  return isInRange(codePoint, CJK_EXTENSION_E_START, CJK_EXTENSION_E_END);
}

inline bool isCJKExtensionF(uint32_t codePoint) {
  return isInRange(codePoint, CJK_EXTENSION_F_START, CJK_EXTENSION_F_END);
}

inline bool isJapanesePunctuation(uint32_t codePoint) {
  return isInRange(codePoint, JAPANESE_PUNCTUATION_START,
                   JAPANESE_PUNCTUATION_END);
}
