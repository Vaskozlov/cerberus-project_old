#include <cerberus/text_scan_assistance/scan_api.hpp>

namespace cerb::text
{
    template struct ScanApiError<char>;
    template struct ScanApiError<char8_t>;
    template struct ScanApiError<char16_t>;
    template struct ScanApiError<char32_t>;
    template struct ScanApiError<wchar_t>;

    template struct ScanApi<char>;
    template struct ScanApi<char8_t>;
    template struct ScanApi<char16_t>;
    template struct ScanApi<char32_t>;
    template struct ScanApi<wchar_t>;

    template struct NotationEscapeSymbol<char>;
    template struct NotationEscapeSymbol<char8_t>;
    template struct NotationEscapeSymbol<char16_t>;
    template struct NotationEscapeSymbol<char32_t>;
    template struct NotationEscapeSymbol<wchar_t>;

    template struct EscapeSymbol<char>;
    template struct EscapeSymbol<char8_t>;
    template struct EscapeSymbol<char16_t>;
    template struct EscapeSymbol<char32_t>;
    template struct EscapeSymbol<wchar_t>;
}// namespace cerb::text
