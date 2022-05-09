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
        ConstBitmap<1, pow2<size_t>(bitsizeof(char))> bitmap;
        GeneratorForText<char> text_generator{ TestInput };
        RegexParser<char> regex_parser{ text_generator, bitmap };

        for (u16 chr : Range<u16>('\0', '\t')) {
            EXPECT_TRUE(bitmap.at<0>(chr) == 0);
        }

        EXPECT_TRUE(bitmap.at<0>(asUInt('\t')));

        for (u16 chr : Range<u16>('\t' + 1, '-')) {
            EXPECT_TRUE(bitmap.at<0>(chr) == 0);
        }

        EXPECT_TRUE(bitmap.at<0>(asUInt('-')));

        for (u16 chr : Range<u16>('-' + 1, '0')) {
            EXPECT_TRUE(bitmap.at<0>(chr) == 0);
        }

        for (u16 chr : Range<u16>('0', '9' + 1)) {
            EXPECT_TRUE(bitmap.at<0>(chr));
        }

        for (u16 chr : Range<u16>('0', '9' + 1)) {
            EXPECT_TRUE(bitmap.at<0>(chr));
        }

        for (u16 chr : Range<u16>('9' + 1, 'A')) {
            EXPECT_TRUE(bitmap.at<0>(chr) == 0);
        }

        for (u16 chr : Range<u16>('A', 'Z' + 1)) {
            EXPECT_TRUE(bitmap.at<0>(chr));
        }

        for (u16 chr : Range<u16>('Z' + 1, '_')) {
            EXPECT_TRUE(bitmap.at<0>(chr) == 0);
        }

        EXPECT_TRUE(bitmap.at<0>(asUInt('_')));

        for (u16 chr : Range<u16>('_' + 1, 'a')) {
            EXPECT_TRUE(bitmap.at<0>(chr) == 0);
        }

        for (u16 chr : Range<u16>('a', 'z' + 1)) {
            EXPECT_TRUE(bitmap.at<0>(chr));
        }

        for (u16 chr : Range<u16>('z' + 1, 0xFF)) {
            EXPECT_TRUE(bitmap.at<0>(chr) == 0);
        }

        return true;
    }

    auto testRegexParser() -> int
    {
        EXPECT_TRUE(testRegexParserOnBasicString());
        return 0;
    }
}// namespace cerb::debug
