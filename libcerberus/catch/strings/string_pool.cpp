#include <cerberus/debug/debug.hpp>
#include <cerberus/string_pool.hpp>

namespace cerb::debug
{
    // NOLINTNEXTLINE
    static const StringPool<char, unsigned> TestStringPool({ { "Hello", 0 }, { "World", 1 } });

    auto testStringPoolContains() -> bool
    {
        StringPool<char, unsigned> string_pool = TestStringPool;

        EXPECT_TRUE(string_pool.contains("Hello"));
        EXPECT_TRUE(string_pool.contains("World"));

        EXPECT_FALSE(string_pool.contains("Test"));
        return true;
    }

    auto testStringPoolSubscript() -> bool
    {
        StringPool<char, unsigned> string_pool = TestStringPool;

        EXPECT_EQUAL(string_pool["Hello"], 0);
        EXPECT_EQUAL(string_pool["World"], 1);

        try {
            CERBLIB_UNUSED(auto) = string_pool["Test"];
            CANT_BE_REACHED;
        } catch (std::out_of_range const &) {
            MUST_BE_REACHED;
        }

        return true;
    }

    auto testStringPoolEmplace() -> bool
    {
        StringPool<char, unsigned> string_pool = TestStringPool;

        string_pool.emplace("Test", 3);
        EXPECT_TRUE(string_pool.contains("Test"));
        EXPECT_TRUE(string_pool["Test"] == 3);

        return true;
    }

    auto testStringPoolFindLongestMatchingString() -> bool
    {
        StringPool<char, unsigned> string_pool = TestStringPool;

        EXPECT_EQUAL(string_pool.findLongestStringInPool("WorldIt'sATest"), "World");
        return true;
    }

    auto testStringPool() -> int
    {
        EXPECT_TRUE(testStringPoolContains());
        EXPECT_TRUE(testStringPoolSubscript());
        EXPECT_TRUE(testStringPoolEmplace());
        EXPECT_TRUE(testStringPoolFindLongestMatchingString());

        return 0;
    }

}// namespace cerb::debug
