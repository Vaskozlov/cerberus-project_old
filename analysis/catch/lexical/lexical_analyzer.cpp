#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

namespace cerb::debug {
    using namespace lex;

    auto testLexicalAnalyzer() -> int {
        CERBLIB_UNUSED(LexicalAnalyzer<char>) = {{0, "[0-9]+p\"for\""}};
        return 0;
    }
}// namespace cerb::debug
