#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/char.hpp>
#include <cerberus/range.hpp>

namespace cerb::debug
{
    using namespace cerb::lex;

    CERBLIB_DECL auto testChar2Hex() -> bool
    {
        for (u16 i : Range<u16>(10)) {
            EXPECT_TRUE(HexadecimalCharsToInt<char>[static_cast<char>('0' + i)] == i);
        }

        for (u16 i : Range<u16>(6)) {
            EXPECT_TRUE(HexadecimalCharsToInt<char>[static_cast<char>('a' + i)] == (i + 10));
        }

        for (u16 i : Range<u16>(6)) {
            EXPECT_TRUE(HexadecimalCharsToInt<char>[static_cast<char>('A' + i)] == (i + 10));
        }

        return true;
    }

    CERBLIB_DECL auto testu16Char2Hex() -> bool
    {
        for (u16 i : Range<u16>(10)) {
            EXPECT_TRUE(HexadecimalCharsToInt<char16_t>[u'0' + i] == i);
        }

        for (u16 i : Range<u16>(6)) {
            EXPECT_TRUE(HexadecimalCharsToInt<char16_t>[u'a' + i] == (i + 10));
        }

        for (u16 i : Range<u16>(6)) {
            EXPECT_TRUE(HexadecimalCharsToInt<char16_t>[u'A' + i] == (i + 10));
        }

        return true;
    }

    CERBLIB_DECL auto testCharEnum() -> bool
    {
        static_assert(CharEnum<char>::EoF == 0);
        static_assert(CharEnum<char>::Tab == '\t');
        static_assert(CharEnum<char>::NewLine == '\n');
        static_assert(CharEnum<char>::CarriageReturn == '\r');
        static_assert(CharEnum<char>::DQM == '\"');
        static_assert(CharEnum<char>::Apostrophe == '\'');
        static_assert(CharEnum<char>::Backlash == '\\');
        static_assert(CharEnum<char>::Space == ' ');
        static_assert(CharEnum<char>::Underscore == '_');

        return true;
    }

    CERBLIB_DECL auto testCharEnumU16() -> bool
    {
        static_assert(CharEnum<char16_t>::EoF == 0);
        static_assert(CharEnum<char16_t>::Tab == u'\t');
        static_assert(CharEnum<char16_t>::NewLine == u'\n');
        static_assert(CharEnum<char16_t>::CarriageReturn == u'\r');
        static_assert(CharEnum<char16_t>::DQM == u'\"');
        static_assert(CharEnum<char16_t>::Apostrophe == u'\'');
        static_assert(CharEnum<char16_t>::Backlash == u'\\');
        static_assert(CharEnum<char16_t>::Space == u' ');
        static_assert(CharEnum<char16_t>::Underscore == u'_');

        return true;
    }

    auto testChar() -> int
    {
        CERBERUS_TEST(testChar2Hex());
        CERBERUS_TEST(testu16Char2Hex());
        CERBERUS_TEST(testCharEnum());
        CERBERUS_TEST(testCharEnumU16());
        return 0;
    }
}// namespace cerb::debug