#include <cerberus/lex/item/basic_item.hpp>

namespace cerb::lex
{
    template struct AnalysisGlobals<char>;
    template struct AnalysisGlobals<char8_t>;
    template struct AnalysisGlobals<char16_t>;

    template struct BasicItem<char>;
    template struct BasicItem<char8_t>;
    template struct BasicItem<char16_t>;
}// namespace cerb::lex