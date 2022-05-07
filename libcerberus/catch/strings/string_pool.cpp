#include <cerberus/debug/debug.hpp>
#include <cerberus/string_pool.hpp>

namespace cerb::debug
{
    auto testStringPool() -> int
    {
        using namespace string_view_literals;

        StringPool<char, unsigned> string_pool({ { "Hello"_sv, 0 }, { "World"_sv, 1 } });

        EXPECT_TRUE(string_pool.contains("Hello"_sv));
        EXPECT_TRUE(string_pool["Hello"_sv] == 0);
        EXPECT_TRUE(string_pool["World"_sv] == 1);
        EXPECT_TRUE(string_pool.findLongestStringInPool("WorldIt'sATest"_sv) == "World"_sv);

        try {
            CERBLIB_UNUSED(auto) = string_pool["Test"_sv];
            CANT_BE_REACHED;
        } catch (std::out_of_range const &) {
            MUST_BE_REACHED;
        }

        string_pool.emplace("Test"_sv, 3);
        EXPECT_TRUE(string_pool.contains("Test"_sv));
        EXPECT_TRUE(string_pool["Test"_sv] == 3);

        return 0;
    }
}// namespace cerb::debug
