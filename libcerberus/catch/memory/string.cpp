#include <cerberus/debug/debug.hpp>
#include <cerberus/memory.hpp>
#include <cerberus/pair.hpp>

namespace cerb::debug
{
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
        std::array integrals = { -10, -20, 400, 14, 0, 123 };

        EXPECT_TRUE(find(integrals, 123) == integrals.end() - 1);
        EXPECT_TRUE(
            find(integrals.data(), 123, integrals.size()) ==
            (integrals.data() + integrals.size() - 1));

        EXPECT_TRUE(find(integrals, 500) >= integrals.end());
        EXPECT_TRUE(
            find(integrals.data(), 500, integrals.size()) >= (integrals.data() + integrals.size()));

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

    auto testStringOperations() -> int
    {
        CERBERUS_TEST(testFindOnStringView());
        CERBERUS_TEST(testFindOnUtf16StringView());
        CERBERUS_TEST(testFindOnWstringView());
        CERBERUS_TEST(testFindOnArrayOfInts());
        CERBERUS_TEST(testFindOnArrayOfPairs());
        return 0;
    }
}// namespace cerb::debug
