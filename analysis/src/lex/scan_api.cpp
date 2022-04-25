#include <cerberus/lex/scan_api.hpp>

namespace cerb::scan
{
    template struct ScanApi<false, char>;
    template struct ScanApi<true, char>;

    template struct ScanApi<false, char8_t>;
    template struct ScanApi<true, char8_t>;

    template struct ScanApi<false, char16_t>;
    template struct ScanApi<true, char16_t>;

    template struct ScanApi<false, char32_t>;
    template struct ScanApi<true, char32_t>;
}// namespace cerb::scan
