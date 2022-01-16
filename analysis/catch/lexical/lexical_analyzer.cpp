#include "lexical_analyzer.hpp"
#include <cerberus/lexical/lexical_analyzer.hpp>

namespace cerb::test {
    auto lexicalAnalyzerTest(u32) -> int
    {
        lex::LexicalAnalyzer<char, u32> lexical_analyzer{
            { '\'', 0 }, { '\"', 1 }, { "//", "/*", "*/" }, { { "\"u\"[0-9]+", 2 } }, {}
        };

        lexical_analyzer.setStream("None", "/**/ u41095");
        CERBLIB_IGNORE lexical_analyzer.yield();

        return 0;
    }
}// namespace cerb::test
