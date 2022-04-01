namespace cerb::debug
{
    auto testChar() -> int;
    auto testToken() -> int;
    auto testStringParser() -> int;
    auto testGeneratorForText() -> int;
    auto testLexicalAnalyzer() -> int;
    auto testStringToCodes() -> int;
    auto testRegexItem() -> int;
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
    testRegexItem();
    return 0;
}
