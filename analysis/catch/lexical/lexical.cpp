namespace cerb::debug
{
    auto testChar() -> int;
    auto testToken() -> int;
    auto testStringParser() -> int;
    auto testGeneratorForText() -> int;
    auto lexicalAnalyzerTest() -> int;
}// namespace cerb::debug


auto main() -> int
{
    using namespace cerb::debug;

    testChar();
    testToken();
    testStringParser();
    testGeneratorForText();
    lexicalAnalyzerTest();

    return 0;
}
