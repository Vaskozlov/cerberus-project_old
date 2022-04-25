#include <cerberus/debug/debug.hpp>

namespace cerb::debug
{
    auto testAbs() -> int;
    auto testPow2() -> int;
    auto testLog2() -> int;
    auto testMinMax() -> int;
    auto testBitmap() -> int;
    auto testBitScan() -> int;
    auto testByteMask() -> void;

    auto testBit() -> int
    {
        testAbs();
        testPow2();
        testLog2();
        testMinMax();
        testByteMask();
        testBitScan();
        return 0;
    }
}// namespace cerb::debug
