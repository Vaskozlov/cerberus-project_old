#include "memory.hpp"
#include <cerberus/memory.hpp>
#include <cerberus/pair.hpp>

namespace cerb::test {
    auto stringTest(u32) -> int
    {
        using namespace std::string_view_literals;

        const std::string_view str                   = "hello, world!"sv;
        const std::u16string_view u16str             = u"hello, world!"sv;
        const std::wstring_view wide_string          = L"hello, world!"sv;
        std::array array_of_integrals                = { -10, -20, 400, 14, 0, 123 };
        std::array<Pair<i32, i32>, 6> array_of_pairs = {
            { { -10, 10 }, { -20, 30 }, { 400, 400 }, { 14, 0 }, { 1, 0 }, { 123, 123 } }
        };

        EXPECT_TRUE(find(str.data(), '\0', str.size() + 1) == str.size());
        EXPECT_TRUE(find(u16str.data(), u'\0', str.size() + 1) == str.size());
        EXPECT_TRUE(find(wide_string.data(), L'\0', str.size() + 1) == str.size());

        EXPECT_TRUE(find(str.data(), '\1', str.size()) >= str.size());
        EXPECT_TRUE(find(u16str.data(), u'\1', str.size()) >= str.size());
        EXPECT_TRUE(find(wide_string.data(), L'\1', str.size()) >= str.size());

        EXPECT_TRUE(find(array_of_integrals.data(), 123) == array_of_integrals.size() - 1);
        EXPECT_TRUE(find(array_of_pairs.data(), { 123, 123 }) == array_of_pairs.size() - 1);
        EXPECT_TRUE(
            find(array_of_integrals.data(), 500, array_of_integrals.size()) >=
            array_of_integrals.size());

        return 0;
    }
}// namespace cerb::test