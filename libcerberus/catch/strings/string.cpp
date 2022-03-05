#include "string.hpp"
#include "string_pool.hpp"
#include "string_view.hpp"
#include <cerberus/debug/debug.hpp>

namespace cerb::debug
{
    auto testStringModule() -> int
    {
        testStringView();
        stringPoolTest();
        return 0;
    }
}// namespace cerb::debug
