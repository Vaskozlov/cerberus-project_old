#include "bit.hpp"
#include "bitmap.hpp"
#include "enum.hpp"
#include "flat_map.hpp"
#include "memory.hpp"
#include "pair.hpp"
#include "pointer_wrapper.hpp"
#include "string_pool.hpp"
#include "string_view.hpp"

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
