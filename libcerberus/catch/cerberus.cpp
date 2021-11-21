#include "bit.hpp"
#include "memory.hpp"
#include "pair.hpp"
#include <cerberus/bitmap.hpp>

auto main() -> int
{
    cerb::test::pair_test();
    cerb::test::bit_test();
    cerb::test::memory_test();
    return 0;
}
