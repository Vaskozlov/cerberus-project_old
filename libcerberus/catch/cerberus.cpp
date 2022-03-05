#include "bits/bit.hpp"
#include "bits/bitmap.hpp"
#include "enum.hpp"
#include "flat_map.hpp"
#include "for_each.hpp"
#include "memory/memory.hpp"
#include "memory/pointer_wrapper.hpp"
#include "pair.hpp"
#include "strings/string.hpp"

auto main() -> int
{
    using namespace cerb::debug;

    testPair();
    testBit();
    memoryTest();
    testStringModule();
    pointerWrapperTest();
    bitmapTest();
    testEnum();
    testFlatMap();
    testForEach();

    return 0;
}
