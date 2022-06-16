#include <cerberus/text/scan_api.hpp>

namespace cerb::text
{
    template struct ScanApiError<char>;
    template struct ScanApiError<char8_t>;
    template struct ScanApiError<char16_t>;
    template struct ScanApiError<char32_t>;
    template struct ScanApiError<wchar_t>;

    template struct ScanApi<RAW_CHARS, char>;
    template struct ScanApi<CLEAN_CHARS, char>;

    template struct ScanApi<RAW_CHARS, char8_t>;
    template struct ScanApi<CLEAN_CHARS, char8_t>;

    template struct ScanApi<RAW_CHARS, char16_t>;
    template struct ScanApi<CLEAN_CHARS, char16_t>;

    template struct ScanApi<RAW_CHARS, char32_t>;
    template struct ScanApi<CLEAN_CHARS, char32_t>;

    template struct ScanApi<RAW_CHARS, wchar_t>;
    template struct ScanApi<CLEAN_CHARS, wchar_t>;

    template struct NotationEscapeSymbol<RAW_CHARS, char>;
    template struct NotationEscapeSymbol<CLEAN_CHARS, char>;

    template struct NotationEscapeSymbol<RAW_CHARS, char8_t>;
    template struct NotationEscapeSymbol<CLEAN_CHARS, char8_t>;

    template struct NotationEscapeSymbol<RAW_CHARS, char16_t>;
    template struct NotationEscapeSymbol<CLEAN_CHARS, char16_t>;

    template struct NotationEscapeSymbol<RAW_CHARS, char32_t>;
    template struct NotationEscapeSymbol<CLEAN_CHARS, char32_t>;

    template struct NotationEscapeSymbol<RAW_CHARS, wchar_t>;
    template struct NotationEscapeSymbol<CLEAN_CHARS, wchar_t>;

    template struct EscapeSymbol<RAW_CHARS, char>;
    template struct EscapeSymbol<CLEAN_CHARS, char>;

    template struct EscapeSymbol<RAW_CHARS, char8_t>;
    template struct EscapeSymbol<CLEAN_CHARS, char8_t>;

    template struct EscapeSymbol<RAW_CHARS, char16_t>;
    template struct EscapeSymbol<CLEAN_CHARS, char16_t>;

    template struct EscapeSymbol<RAW_CHARS, char32_t>;
    template struct EscapeSymbol<CLEAN_CHARS, char32_t>;

    template struct EscapeSymbol<RAW_CHARS, wchar_t>;
    template struct EscapeSymbol<CLEAN_CHARS, wchar_t>;
}// namespace cerb::text
