#include <cerberus/debug/debug.hpp>
#include <cerberus/memory.hpp>
#include <cerberus/pair.hpp>

namespace cerb::debug
{
    constexpr std::array TestIntegrals = { -10, 0, 400, 14, 0, 123 };

    CERBLIB_DECL auto testFindOnStringView() -> bool
    {
        std::string_view str = "hello, world!";

        EXPECT_TRUE(find(str, '\0') == str.end());
        EXPECT_TRUE(find(str, '\1') >= str.end());

        return true;
    }

    CERBLIB_DECL auto testFindOnUtf16StringView() -> bool
    {
        std::u16string_view str = u"hello, world!";

        EXPECT_TRUE(find(str.data(), u'\0', str.size()) == (str.data() + str.size()));
        EXPECT_TRUE(find(str.data(), u'\1', str.size()) >= (str.data() + str.size()));
        EXPECT_TRUE(find(str, u'\0') == str.end());
        EXPECT_TRUE(find(str, u'\1') >= str.end());

        return true;
    }

    CERBLIB_DECL auto testFindOnWstringView() -> bool
    {
        std::wstring_view str = L"hello, world!";

        EXPECT_TRUE(find(str.data(), L'\0', str.size()) == (str.data() + str.size()));
        EXPECT_TRUE(find(str.data(), L'\1', str.size()) >= (str.data() + str.size()));
        EXPECT_TRUE(find(str, L'\0') == str.end());
        EXPECT_TRUE(find(str, L'\1') >= str.end());

        return true;
    }

    CERBLIB_DECL auto testFindOnArrayOfInts() -> bool
    {
        EXPECT_TRUE(find(TestIntegrals, 123) == TestIntegrals.end() - 1);
        EXPECT_TRUE(
            find(TestIntegrals.data(), 123, TestIntegrals.size()) ==
            (TestIntegrals.data() + TestIntegrals.size() - 1));

        EXPECT_TRUE(find(TestIntegrals, 500) >= TestIntegrals.end());
        EXPECT_TRUE(
            find(TestIntegrals.data(), 500, TestIntegrals.size()) >=
            (TestIntegrals.data() + TestIntegrals.size()));

        return true;
    }

    CERBLIB_DECL auto testFindOnArrayOfPairs() -> bool
    {
        std::array<Pair<i32, i32>, 6> pairs = {
            { { -10, 10 }, { -20, 30 }, { 400, 400 }, { 14, 0 }, { 1, 0 }, { 123, 123 } }
        };

        EXPECT_TRUE(find(pairs, { 123, 123 }) == pairs.end() - 1);
        EXPECT_TRUE(find(pairs, { 123, 123 }) == pairs.end() - 1);

        return true;
    }

    CERBLIB_DECL auto testRfindOnStringView() -> bool
    {
        std::string_view str = "hello, world!";

        return rfind(str, '\0') == str.rend();
    }

    CERBLIB_DECL auto testRfindOnArrayOfInts() -> bool
    {
        auto last_zero_index = TestIntegrals.rbegin() + 1;
        auto index = rfind(TestIntegrals, 0);

        return index == last_zero_index;
    }

    CERBLIB_DECL auto testRfindOnArrayOfIntsWithNoSuitableInts() -> bool
    {
        auto last_zero_index = TestIntegrals.rend();
        auto index = rfind(TestIntegrals, -100);

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
