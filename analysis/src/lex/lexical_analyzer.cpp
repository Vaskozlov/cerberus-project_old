#include <cerberus/lex/lexical_analyzer.hpp>

namespace cerb::lex
{
    template class LexicalAnalyzer<char>;
    template class LexicalAnalyzer<char8_t>;
    template class LexicalAnalyzer<char16_t>;
}// namespace cerb::lex
