namespace cerb::debug
{
    auto testEnum() -> int;
    auto testFlatMap() -> int;
    auto testForEach() -> int;
    auto testPair() -> int;
    auto testRange() -> int;

    auto testBit() -> int;
    auto memoryTest() -> int;
    auto testStringModule() -> int;

    auto testFmt() -> int;
}// namespace cerb::debug

auto main() -> int
{
    using namespace cerb::debug;

    testEnum();
    testFlatMap();
    testForEach();
    testPair();
    testRange();

    testBit();
    memoryTest();
    testStringModule();

    testFmt();

    return 0;
}
