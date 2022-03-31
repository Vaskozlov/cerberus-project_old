namespace cerb::debug
{
    auto testChar() -> int;
    auto testToken() -> int;
    auto testStringParser() -> int;
    auto testGeneratorForText() -> int;
    auto lexicalAnalyzerTest() -> int;
    auto testStringToCodes() -> int;
}// namespace cerb::debug

auto main() -> int
{
    using namespace cerb::debug;

    testChar();
    testToken();
    testStringParser();
    testGeneratorForText();
    lexicalAnalyzerTest();
    testStringToCodes();

    return 0;
}
