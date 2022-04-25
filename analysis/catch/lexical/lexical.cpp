namespace cerb::debug
{
    auto testChar() -> int;
    auto testToken() -> int;
    auto testStringParser() -> int;
    auto testGeneratorForText() -> int;
    auto testLexicalAnalyzer() -> int;
    auto testStringToCodes() -> int;

    auto testDotItem() -> int;
    auto testRegexParser() -> int;
    auto testBracketFinder() -> int;
}// namespace cerb::debug

auto main() -> int
{
    using namespace cerb::debug;

    testChar();
    testToken();
    testStringParser();
    testGeneratorForText();
    testLexicalAnalyzer();
    testStringToCodes();

    testDotItem();
    testRegexParser();
    testBracketFinder();

    return 0;
}
