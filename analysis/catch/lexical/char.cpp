#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/char.hpp>

namespace cerb::debug
{
    using namespace cerb::lex;

    CERBLIB_DECL auto testChar2Hex() -> bool
    {
        for (u16 i = 0; i < 10; ++i) {
            EXPECT_TRUE(HexadecimalCharsToInt<char>[static_cast<char>('0' + i)] == i);
        }

        for (u16 i = 0; i < 6; ++i) {
            EXPECT_TRUE(HexadecimalCharsToInt<char>[static_cast<char>('a' + i)] == (i + 10));
        }

        for (u16 i = 0; i < 6; ++i) {
            EXPECT_TRUE(HexadecimalCharsToInt<char>[static_cast<char>('A' + i)] == (i + 10));
        }

        return true;
    }

    CERBLIB_DECL auto testu16Char2Hex() -> bool
    {
        for (u16 i = 0; i < 10; ++i) {
            EXPECT_TRUE(HexadecimalCharsToInt<char16_t>[u'0' + i] == i);
        }

        for (u16 i = 0; i < 6; ++i) {
            EXPECT_TRUE(HexadecimalCharsToInt<char16_t>[u'a' + i] == (i + 10));
        }

        for (u16 i = 0; i < 6; ++i) {
            EXPECT_TRUE(HexadecimalCharsToInt<char16_t>[u'A' + i] == (i + 10));
        }

        return true;
    }

    auto testCharEnum() -> void
    {
        static_assert(CharEnum<char>::EoF == 0);
        static_assert(CharEnum<char>::Tab == '\t');
        static_assert(CharEnum<char>::NewLine == '\n');
        static_assert(CharEnum<char>::CarriageReturn == '\r');
        static_assert(CharEnum<char>::DQM == '\"');
        static_assert(CharEnum<char>::Apostrophe == '\'');
        static_assert(CharEnum<char>::Backslash == '\\');
        static_assert(CharEnum<char>::Space == ' ');
        static_assert(CharEnum<char>::Underscore == '_');
    }

    auto testCharEnumU16() -> void
    {
        static_assert(CharEnum<char16_t>::EoF == 0);
        static_assert(CharEnum<char16_t>::Tab == u'\t');
        static_assert(CharEnum<char16_t>::NewLine == u'\n');
        static_assert(CharEnum<char16_t>::CarriageReturn == u'\r');
        static_assert(CharEnum<char16_t>::DQM == u'\"');
        static_assert(CharEnum<char16_t>::Apostrophe == u'\'');
        static_assert(CharEnum<char16_t>::Backslash == u'\\');
        static_assert(CharEnum<char16_t>::Space == u' ');
        static_assert(CharEnum<char16_t>::Underscore == u'_');
    }

    auto testChar() -> int
    {
        CR_EXPECT_TRUE(testChar2Hex());
        CR_EXPECT_TRUE(testu16Char2Hex());
        testCharEnum();
        testCharEnumU16();
        return 0;
    }
}// namespace cerb::debug