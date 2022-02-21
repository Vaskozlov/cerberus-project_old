#include "memory.hpp"
#include <cerberus/memory.hpp>
#include <cerberus/pair.hpp>

namespace cerb::debug
{
    auto stringViewFindTest() -> void
    {
        using namespace std::string_view_literals;

        std::string_view const str = "hello, world!"sv;
        EXPECT_TRUE(find(str, '\0') == str.end());
        EXPECT_TRUE(find(str, '\1') >= str.end());
    }

    auto u16StringViewFindTest() -> void
    {
        using namespace std::string_view_literals;

        std::u16string_view const str = u"hello, world!"sv;
        EXPECT_TRUE(find(str.data(), u'\0', str.size()) == (str.data() + str.size()));
        EXPECT_TRUE(find(str.data(), u'\1', str.size()) >= (str.data() + str.size()));
        EXPECT_TRUE(find(str, u'\0') == str.end());
        EXPECT_TRUE(find(str, u'\1') >= str.end());
    }

    auto wstringViewFindTest() -> void
    {
        using namespace std::string_view_literals;

        std::wstring_view const str = L"hello, world!"sv;
        EXPECT_TRUE(find(str.data(), L'\0', str.size()) == (str.data() + str.size()));
        EXPECT_TRUE(find(str.data(), L'\1', str.size()) >= (str.data() + str.size()));
        EXPECT_TRUE(find(str, L'\0') == str.end());
        EXPECT_TRUE(find(str, L'\1') >= str.end());
    }

    auto arrayOfIntFindTest() -> void
    {
        std::array array_of_integrals = { -10, -20, 400, 14, 0, 123 };

        EXPECT_TRUE(find(array_of_integrals, 123) == array_of_integrals.end() - 1);
        EXPECT_TRUE(
            find(array_of_integrals.data(), 123, array_of_integrals.size()) ==
            (array_of_integrals.data() + array_of_integrals.size() - 1));

        EXPECT_TRUE(find(array_of_integrals, 500) >= array_of_integrals.end());
        EXPECT_TRUE(
            find(array_of_integrals.data(), 500, array_of_integrals.size()) >=
            (array_of_integrals.data() + array_of_integrals.size()));
    }

    auto arrayOfPairsFindTest() -> void
    {
        std::array<Pair<i32, i32>, 6> array_of_pairs = {
            { { -10, 10 }, { -20, 30 }, { 400, 400 }, { 14, 0 }, { 1, 0 }, { 123, 123 } }
        };

        EXPECT_TRUE(find(array_of_pairs, { 123, 123 }) == array_of_pairs.end() - 1);
        EXPECT_TRUE(find(array_of_pairs, { 123, 123 }) == array_of_pairs.end() - 1);
    }

    auto stringTest(u32) -> int
    {
        stringViewFindTest();
        u16StringViewFindTest();
        wstringViewFindTest();
        arrayOfIntFindTest();
        arrayOfPairsFindTest();
        return 0;
    }
}// namespace cerb::test
