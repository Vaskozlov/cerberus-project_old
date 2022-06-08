#include <cerberus/debug/debug.hpp>
#include <cerberus/string_container.hpp>

namespace cerb::debug
{
    using namespace string_view_literals;

    // NOLINTNEXTLINE
    static const StringContainer<char, unsigned> TestStringPool({ { "Hello", 0 }, { "World", 1 } });

    auto testStringPoolContains() -> bool
    {
        StringContainer<char, unsigned> string_pool = TestStringPool;

        ASSERT_TRUE(string_pool.contains("Hello"_sv));
        ASSERT_TRUE(string_pool.contains("World"_sv));

        ASSERT_FALSE(string_pool.contains("Test"_sv));
        return true;
    }

    auto testStringPoolSubscript() -> bool
    {
        StringContainer<char, unsigned> string_pool = TestStringPool;

        ASSERT_EQUAL(string_pool["Hello"], 0);
        ASSERT_EQUAL(string_pool["World"], 1);

        ERROR_EXPECTED(
            CERBLIB_UNUSED(auto) = string_pool["Test"], std::out_of_range,
            "Cerberus string pool does not contains given element!")

        return true;
    }

    auto testStringPoolEmplace() -> bool
    {
        StringContainer<char, unsigned> string_pool = TestStringPool;

        string_pool.emplace("Test", 3);
        ASSERT_TRUE(string_pool.contains("Test"_sv));
        ASSERT_TRUE(string_pool["Test"] == 3);

        return true;
    }

    auto testStringPoolFindLongestMatchingString() -> bool
    {
        StringContainer<char, unsigned> string_pool = TestStringPool;

        ASSERT_EQUAL(string_pool.findLongestString("WorldIt'sATest"_sv), "World");
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
