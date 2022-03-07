#include "memory.hpp"
#include "copy.hpp"
#include "equal.hpp"
#include "fill.hpp"
#include "pointer_wrapper.hpp"

namespace cerb::debug
{
    auto testStringOperations(u32) -> int;

    auto memoryTest(u32 argc) -> int
    {
        testFill();
        testCopy();
        testEqual();
        testStringOperations(argc);
        testPointerWrapper();
        return 0;
    }
}// namespace cerb::debug
