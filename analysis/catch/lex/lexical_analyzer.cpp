#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

namespace cerb::debug
{
    using namespace lex;

    auto testLexicalAnalyzer() -> int
    {
        LexicalAnalyzer<char> lexical_analyzer = { { "dot", "\'.\'" },
                                                   { "int", "[0-9]+" },
                                                   { "double", "[0-9]+\".\"[0-9]*" } };

        return 0;
    }
}// namespace cerb::debug
