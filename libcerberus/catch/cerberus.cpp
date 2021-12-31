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

    cerb::ConstBitmap<2, 512> bitmap{};
    bitmap.clear();
    bitmap.set<1>(10);
    fmt::print("{}, {}\n", bitmap.at<0>(10), bitmap.at<0>(0));

    return 0;
}
