#include <cerberus/lex/item/basic_item.hpp>

namespace cerb::lex
{
    template struct AnalysisGlobalsParameters<char>;
    template struct AnalysisGlobalsParameters<char8_t>;
    template struct AnalysisGlobalsParameters<char16_t>;

    template struct BasicItem<char>;
    template struct BasicItem<char8_t>;
    template struct BasicItem<char16_t>;
}// namespace cerb::lex
