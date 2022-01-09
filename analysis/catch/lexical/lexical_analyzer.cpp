#include "lexical_analyzer.hpp"
#include <cerberus/lexical/lexical_analyzer.hpp>

namespace cerb::test {
    auto lexicalAnalyzerTest(u32) -> int
    {
        lex::LexicalAnalyzer<char, u32> lexical_analyzer{
            { '\'', 0 }, { '\"', 1 }, { "//", "/*", "*/" }, { { "\"Hello\"pr[a-z]*", 2 } }
        };

        return 0;
    }
}// namespace cerb::test
