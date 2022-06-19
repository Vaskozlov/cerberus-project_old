#include <cerberus/text_scan_assistance/scan_api.hpp>

namespace cerb::text
{
    template struct ScanApiError<char>;
    template struct ScanApiError<char8_t>;
    template struct ScanApiError<char16_t>;
    template struct ScanApiError<char32_t>;
    template struct ScanApiError<wchar_t>;

    template class ScanApi<char>;
    template class ScanApi<char8_t>;
    template class ScanApi<char16_t>;
    template class ScanApi<char32_t>;
    template class ScanApi<wchar_t>;

    template struct NotationEscapeSymbol<char>;
    template struct NotationEscapeSymbol<char8_t>;
    template struct NotationEscapeSymbol<char16_t>;
    template struct NotationEscapeSymbol<char32_t>;
    template struct NotationEscapeSymbol<wchar_t>;

    template class EscapeSymbol<char>;
    template class EscapeSymbol<char8_t>;
    template class EscapeSymbol<char16_t>;
    template class EscapeSymbol<char32_t>;
    template class EscapeSymbol<wchar_t>;
}// namespace cerb::text
