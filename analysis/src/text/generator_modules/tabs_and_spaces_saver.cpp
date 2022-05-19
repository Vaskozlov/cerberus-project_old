#include <cerberus/text/generator_modules/tabs_and_spaces_saver.hpp>

namespace cerb::text::gen
{
    template struct TabsAndSpacesSaver<char>;
    template struct TabsAndSpacesSaver<char8_t>;
    template struct TabsAndSpacesSaver<char16_t>;
    template struct TabsAndSpacesSaver<char32_t>;
    template struct TabsAndSpacesSaver<wchar_t>;
}// namespace cerb::text::gen
