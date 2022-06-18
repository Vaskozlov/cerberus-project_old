#include <cerberus/text_scan_assistance/string_to_codes.hpp>

namespace cerb::text
{
    template class StringToCodes<char>;
    template class StringToCodes<char8_t>;
    template class StringToCodes<char16_t>;
    template class StringToCodes<char32_t>;
    template class StringToCodes<wchar_t>;
}// namespace cerb::text
