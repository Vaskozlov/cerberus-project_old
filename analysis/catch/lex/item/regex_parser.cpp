#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/item/regex_parser.hpp>

namespace cerb::debug
{
    using namespace lex;
    using namespace regex;
    using namespace text;

    static constexpr cerb::string_view TestInput = "[\\t\\-_0-9A-Za-z]";

    auto testRegexParserOnBasicString() -> bool
    {
        Bitmap bitmap;
        GeneratorForText text_generator{ TestInput };
        RegexParser regex_parser{ text_generator, bitmap };

        for (u16 chr : Range<u16>('\0', '\t')) {
            ASSERT_TRUE(bitmap.at(chr) == 0);
        }

        ASSERT_TRUE(bitmap.at(asUInt('\t')));

        for (u16 chr : Range<u16>('\t' + 1, '-')) {
            ASSERT_TRUE(bitmap.at(chr) == 0);
        }

        ASSERT_TRUE(bitmap.at(asUInt('-')));

        for (u16 chr : Range<u16>('-' + 1, '0')) {
            ASSERT_TRUE(bitmap.at(chr) == 0);
        }

        for (u16 chr : Range<u16>('0', '9' + 1)) {
            ASSERT_TRUE(bitmap.at(chr));
        }

        for (u16 chr : Range<u16>('0', '9' + 1)) {
            ASSERT_TRUE(bitmap.at(chr));
        }

        for (u16 chr : Range<u16>('9' + 1, 'A')) {
            ASSERT_TRUE(bitmap.at(chr) == 0);
        }

        for (u16 chr : Range<u16>('A', 'Z' + 1)) {
            ASSERT_TRUE(bitmap.at(chr) == 1);
        }

        for (u16 chr : Range<u16>('Z' + 1, '_')) {
            ASSERT_TRUE(bitmap.at(chr) == 0);
        }

        ASSERT_TRUE(bitmap.at(asUInt('_')));

        for (u16 chr : Range<u16>('_' + 1, 'a')) {
            ASSERT_TRUE(bitmap.at(chr) == 0);
        }

        for (u16 chr : Range<u16>('a', 'z' + 1)) {
            ASSERT_TRUE(bitmap.at(chr));
        }

        for (u16 chr : Range<u16>('z' + 1, 0xFF)) {
            ASSERT_TRUE(bitmap.at(chr) == 0);
        }

        return true;
    }

    auto testRegexParser() -> int
    {
        ASSERT_TRUE(testRegexParserOnBasicString());
        return 0;
    }
}// namespace cerb::debug
