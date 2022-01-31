#include "bits/bit.hpp"
#include "bits/bitmap.hpp"
#include "enum.hpp"
#include "flat_map.hpp"
#include "memory/memory.hpp"
#include "memory/pointer_wrapper.hpp"
#include "pair.hpp"
#include "strings/string_pool.hpp"
#include "strings/string_view.hpp"

auto main() -> int
{
    using namespace cerb::test;

    pairTest();
    bitTest();
    memoryTest();
    stringViewTest();
    pointerWrapperTest();
    bitmapTest();
    stringPoolTest();
    enumTest();
    flatMapTest();

    return 0;
}
