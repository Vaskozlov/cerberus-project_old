#include <cerberus/text/generator_for_text.hpp>

namespace cerb::text
{
    template class GeneratorForText<char>;
    template class GeneratorForText<char8_t>;
    template class GeneratorForText<char16_t>;
    template class GeneratorForText<char32_t>;
    template class GeneratorForText<wchar_t>;
}// namespace cerb::text
