#include <cerberus/text/scan_api.hpp>

namespace cerb::text
{
    template struct ScanApiError<char>;
    template struct ScanApiError<char8_t>;
    template struct ScanApiError<char16_t>;
    template struct ScanApiError<char32_t>;
    template struct ScanApiError<wchar_t>;

    template struct ScanApi<false, char>;
    template struct ScanApi<true, char>;

    template struct ScanApi<false, char8_t>;
    template struct ScanApi<true, char8_t>;

    template struct ScanApi<false, char16_t>;
    template struct ScanApi<true, char16_t>;

    template struct ScanApi<false, char32_t>;
    template struct ScanApi<true, char32_t>;

    template struct ScanApi<false, wchar_t>;
    template struct ScanApi<true, wchar_t>;

    template struct NotationEscapeSymbol<false, char>;
    template struct NotationEscapeSymbol<true, char>;

    template struct NotationEscapeSymbol<false, char8_t>;
    template struct NotationEscapeSymbol<true, char8_t>;

    template struct NotationEscapeSymbol<false, char16_t>;
    template struct NotationEscapeSymbol<true, char16_t>;

    template struct NotationEscapeSymbol<false, char32_t>;
    template struct NotationEscapeSymbol<true, char32_t>;

    template struct NotationEscapeSymbol<false, wchar_t>;
    template struct NotationEscapeSymbol<true, wchar_t>;

    template struct EscapeSymbol<false, char>;
    template struct EscapeSymbol<true, char>;

    template struct EscapeSymbol<false, char8_t>;
    template struct EscapeSymbol<true, char8_t>;

    template struct EscapeSymbol<false, char16_t>;
    template struct EscapeSymbol<true, char16_t>;

    template struct EscapeSymbol<false, char32_t>;
    template struct EscapeSymbol<true, char32_t>;

    template struct EscapeSymbol<false, wchar_t>;
    template struct EscapeSymbol<true, wchar_t>;
}// namespace cerb::text
