#include "bit.hpp"
#include "memory.hpp"
#include "pair.hpp"
#include "string_view.hpp"
#include <cerberus/bitmap.hpp>
#include <cerberus/string_view.hpp>

auto main() -> int
{
    cerb::test::pairTest();
    cerb::test::bitTest();
    cerb::test::memoryTest();
    cerb::test::stringViewTest();
    return 0;
}
