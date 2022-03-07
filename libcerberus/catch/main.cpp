#include "bits/bit.hpp"
#include "memory/memory.hpp"
#include "strings/string.hpp"

namespace cerb::debug
{
    auto testEnum() -> int;
    auto testFlatMap() -> int;
    auto testForEach() -> int;
    auto testPair() -> int;
}// namespace cerb::debug

auto main() -> int
{
    using namespace cerb::debug;

    testPair();
    testBit();
    memoryTest();
    testStringModule();
    testEnum();
    testFlatMap();
    testForEach();

    return 0;
}
