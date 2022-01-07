#include "char.hpp"
#include "lexical_analyzer.hpp"
#include "location.hpp"
#include "token.hpp"

auto main() -> int
{
    using namespace cerb::test;

    lexicalCharTest();
    txtManagerTest();
    tokenTest();
    lexicalAnalyzerTest();

    return 0;
}