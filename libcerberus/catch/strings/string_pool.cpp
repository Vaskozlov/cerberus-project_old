#include <cerberus/debug/debug.hpp>
#include <cerberus/string_pool.hpp>

namespace cerb::debug
{
    // NOLINTNEXTLINE
    static const StringPool<char, unsigned> TestStringPool({ { "Hello", 0 }, { "World", 1 } });

    auto testStringPoolContains() -> bool
    {
        StringPool<char, unsigned> string_pool = TestStringPool;

        ASSERT_TRUE(string_pool.contains("Hello"));
        ASSERT_TRUE(string_pool.contains("World"));

        ASSERT_FALSE(string_pool.contains("Test"));
        return true;
    }

    auto testStringPoolSubscript() -> bool
    {
        StringPool<char, unsigned> string_pool = TestStringPool;

        ASSERT_EQUAL(string_pool["Hello"], 0);
        ASSERT_EQUAL(string_pool["World"], 1);

        ERROR_EXPECTED(
            CERBLIB_UNUSED(auto) = string_pool["Test"], std::out_of_range,
            "Cerberus string pool does not contains given element!")

        return true;
    }

    auto testStringPoolEmplace() -> bool
    {
        StringPool<char, unsigned> string_pool = TestStringPool;

        string_pool.emplace("Test", 3);
        ASSERT_TRUE(string_pool.contains("Test"));
        ASSERT_TRUE(string_pool["Test"] == 3);

        return true;
    }

    auto testStringPoolFindLongestMatchingString() -> bool
    {
        StringPool<char, unsigned> string_pool = TestStringPool;

        ASSERT_EQUAL(string_pool.findLongestStringInPool("WorldIt'sATest"), "World");
        return true;
    }

    auto testStringPool() -> int
    {
        ASSERT_TRUE(testStringPoolContains());
        ASSERT_TRUE(testStringPoolSubscript());
        ASSERT_TRUE(testStringPoolEmplace());
        ASSERT_TRUE(testStringPoolFindLongestMatchingString());

        return 0;
    }

}// namespace cerb::debug
