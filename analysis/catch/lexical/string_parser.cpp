#include <cerberus/debug/debug.hpp>

namespace cerb::debug
{
    auto testStringParserOnString() -> void
    {
        // empty
    }

    auto testStringParserOnUtf16String() -> void
    {
        // empty
    }

    auto testStringParser() -> int
    {
        testStringParserOnString();
        testStringParserOnUtf16String();
        return 0;
    }
}// namespace cerb::debug
