#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/char.hpp>
#include <cerberus/range.hpp>

namespace cerb::debug
{
    using namespace cerb::lex;

    static_assert(CharEnum<char>::EoF == '\0');
    static_assert(CharEnum<char>::Tab == '\t');
    static_assert(CharEnum<char>::NewLine == '\n');
    static_assert(CharEnum<char>::CarriageReturn == '\r');
    static_assert(CharEnum<char>::DQM == '\"');
    static_assert(CharEnum<char>::Apostrophe == '\'');
    static_assert(CharEnum<char>::Backlash == '\\');
    static_assert(CharEnum<char>::Space == ' ');
    static_assert(CharEnum<char>::Underscore == '_');

    static_assert(CharEnum<char16_t>::EoF == u'\0');
    static_assert(CharEnum<char16_t>::Tab == u'\t');
    static_assert(CharEnum<char16_t>::NewLine == u'\n');
    static_assert(CharEnum<char16_t>::CarriageReturn == u'\r');
    static_assert(CharEnum<char16_t>::DQM == u'\"');
    static_assert(CharEnum<char16_t>::Apostrophe == u'\'');
    static_assert(CharEnum<char16_t>::Backlash == u'\\');
    static_assert(CharEnum<char16_t>::Space == u' ');
    static_assert(CharEnum<char16_t>::Underscore == u'_');

    CERBERUS_TEST_FUNC(testChar2Hex)
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

    CERBERUS_TEST_FUNC(testU16Char2Hex)
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

    auto testChar() -> int
    {
        CERBERUS_TEST(testChar2Hex());
        CERBERUS_TEST(testU16Char2Hex());

        return 0;
    }
}// namespace cerb::debug