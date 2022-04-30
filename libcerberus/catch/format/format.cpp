#include <cerberus/format/format.hpp>
#include <fmt/format.h>

namespace cerb::debug
{
    auto testFmt() -> int
    {
        string_view str = "Hello, World!";
        std::vector<int> a = { 10, 20, 30, 40 };
        std::vector<int> b{};

        ::fmt::print("{}\n", fmt::format<char>("Hello {} {} {}", 10, '1', a));
        return 0;
    }
}// namespace cerb::debug
