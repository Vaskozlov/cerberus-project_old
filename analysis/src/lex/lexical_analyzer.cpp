#include <cerberus/lex/lexical_analyzer.hpp>

namespace cerb::lex
{
    template struct LexicalAnalyzer<char>;
    template struct LexicalAnalyzer<char8_t>;
    template struct LexicalAnalyzer<char16_t>;
}// namespace cerb::lex
