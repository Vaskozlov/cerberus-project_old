#include "memory.hpp"
#include <cerberus/memory.hpp>
#include <cerberus/pair.hpp>

namespace cerb::test {
    auto string_test(u32) -> int
    {
        expect_true(
            []() {
                const std::string_view str = "hello, world!";
                return find(str.data(), '\0') == str.size();
            },
            CERBLIB_LOCATION);

        expect_true(
            []() {
                const std::u16string_view str = u"hello, world!";
                return find(str.data(), u'\0') == str.size();
            },
            CERBLIB_LOCATION);

        expect_true(
            []() {
                const std::wstring_view str = L"hello, world!";
                return find(str.data(), L'\0') == str.size();
            },
            CERBLIB_LOCATION);

        expect_true(
            []() {
                std::array data = { -10, -20, 400, 14, 0, 123 };
                return find(data.data(), 123) == data.size() - 1;
            },
            CERBLIB_LOCATION);

        expect_true(
            []() {
                std::array<Pair<i32, i32>, 7> data = { { -10, 10 },  { -20, 30 },
                                                       { 400, 400 }, { 14, 0 },
                                                       { 1, 0 },     { 123, 123 } };
                return find(data.data(), { 123, 123 }) == data.size() - 1;
            },
            CERBLIB_LOCATION);

        return 0;
    }
}// namespace cerb::test