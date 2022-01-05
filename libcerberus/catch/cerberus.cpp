#include "bit.hpp"
#include "bitmap.hpp"
#include "memory.hpp"
#include "pair.hpp"
#include "pointer_wrapper.hpp"
#include "string_view.hpp"
#include "string_pool.hpp"

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

    return 0;
}
