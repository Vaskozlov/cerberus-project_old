#include <cerberus/text_scan_assistance/generator_modules/tabs_and_spaces_saver.hpp>

namespace cerb::text::gen
{
    template class TabsAndSpacesSaver<char>;
    template class TabsAndSpacesSaver<char8_t>;
    template class TabsAndSpacesSaver<char16_t>;
    template class TabsAndSpacesSaver<char32_t>;
    template class TabsAndSpacesSaver<wchar_t>;
}// namespace cerb::text::gen
