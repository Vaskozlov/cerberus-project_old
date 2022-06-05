#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

namespace cerb::debug
{
    using namespace lex;

    auto testLexicalAnalyzer() -> int
    {
        auto completion = [](Token<char> const &) {};

        LexicalAnalyzer<char> lexical_analyzer = { { "dot", "\'.\'", completion },
                                                   { "int", "[0-9]+", completion },
                                                   { "double", "[0-9]+\".\"[0-9]*", completion } };


        lexical_analyzer.addSource("1010 . 1010.01");

        return 0;
    }
}// namespace cerb::debug
