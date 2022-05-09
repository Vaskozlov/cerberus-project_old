#include <cerberus/text/generator_for_text.hpp>

namespace cerb::text
{
    template struct GeneratorForText<char>;
    template struct GeneratorForText<char8_t>;
    template struct GeneratorForText<char16_t>;
    template struct GeneratorForText<char32_t>;
    template struct GeneratorForText<wchar_t>;
}// namespace cerb::text
