#include "char.hpp"
#include "lexical_analyzer.hpp"
#include "string_parser.hpp"
#include "text_generator.hpp"
#include "token.hpp"

auto main() -> int
{
    using namespace cerb::test;

    lexicalCharTest();
    txtManagerTest();
    tokenTest();
    lexicalAnalyzerTest();
    stringParserTest();

    return 0;
}
