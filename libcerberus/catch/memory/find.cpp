#include <cerberus/debug/debug.hpp>
#include <cerberus/memory.hpp>
#include <cerberus/pair.hpp>

namespace cerb::debug
{
    constexpr std::array TestIntegrals = { -10, 0, 400, 14, 0, 123 };

    CERBERUS_TEST_FUNC(testFindOnStringView)
    {
        std::string_view str = "hello, world!";

        ASSERT_EQUAL(find(str, '\0'), str.end());

        return true;
    }

    CERBERUS_TEST_FUNC(testFindOnUtf16StringView)
    {
        std::u16string_view str = u"hello, world!";

        ASSERT_EQUAL(find(str.data(), u'\0', str.size()), (str.data() + str.size()));
        ASSERT_EQUAL(find(str, u'\0'), str.end());

        return true;
    }

    CERBERUS_TEST_FUNC(testFindOnWstringView)
    {
        std::wstring_view str = L"hello, world!";

        ASSERT_EQUAL(find(str.data(), L'\0', str.size()), str.data() + str.size());
        ASSERT_EQUAL(find(str, L'\0'), str.end());

        return true;
    }

    CERBERUS_TEST_FUNC(testFindOnArrayOfInts)
    {
        ASSERT_EQUAL(find(TestIntegrals, 123), TestIntegrals.end() - 1);
        ASSERT_EQUAL(
            find(TestIntegrals.data(), 123, TestIntegrals.size()),
            TestIntegrals.data() + TestIntegrals.size() - 1);

        ASSERT_EQUAL(find(TestIntegrals, 500), TestIntegrals.end());
        ASSERT_EQUAL(
            find(TestIntegrals.data(), 500, TestIntegrals.size()),
            TestIntegrals.data() + TestIntegrals.size());

        return true;
    }

    CERBERUS_TEST_FUNC(testFindOnArrayOfPairs)
    {
        // NOLINTBEGIN
        std::array<Pair<i32, i32>, 6> pairs = {
            { { -10, 10 }, { -20, 30 }, { 400, 400 }, { 14, 0 }, { 1, 0 }, { 123, 123 } }
        };

        ASSERT_EQUAL(find(pairs, { 123, 123 }), pairs.end() - 1);
        ASSERT_EQUAL(find(pairs, { -10, 10 }), pairs.begin());
        ASSERT_EQUAL(find(pairs, { 0, 0 }), pairs.end());
        // NOLINTEND

        return true;
    }

    CERBERUS_TEST_FUNC(testRfindOnStringView)
    {
        std::string_view str = "hello, world!";

        return rfind(str, '\0') == str.rend();
    }

    CERBERUS_TEST_FUNC(testRfindOnArrayOfInts)
    {
        auto last_zero_index = TestIntegrals.rbegin() + 1;
        auto index = rfind(TestIntegrals, 0);

        return index == last_zero_index;
    }

    CERBERUS_TEST_FUNC(testRfindOnArrayOfIntsWithNoSuitableInts)
    {
        auto last_zero_index = TestIntegrals.rend();
        auto index = rfind(TestIntegrals, -100);// NOLINT

        return index == last_zero_index;
    }

    auto testFind() -> int
    {
        CERBERUS_TEST(testFindOnStringView());
        CERBERUS_TEST(testFindOnUtf16StringView());
        CERBERUS_TEST(testFindOnWstringView());
        CERBERUS_TEST(testFindOnArrayOfInts());
        CERBERUS_TEST(testFindOnArrayOfPairs());
        CERBERUS_TEST(testRfindOnStringView());
        CERBERUS_TEST(testRfindOnArrayOfInts());
        CERBERUS_TEST(testRfindOnArrayOfIntsWithNoSuitableInts());
        return 0;
    }
}// namespace cerb::debug
