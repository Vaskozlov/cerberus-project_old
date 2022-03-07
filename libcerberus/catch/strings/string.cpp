#include "string.hpp"
#include "string_pool.hpp"
#include "string_view.hpp"

namespace cerb::debug
{
    auto testStringModule() -> int
    {
        testStringView();
        testStringPool();
        return 0;
    }
}// namespace cerb::debug
