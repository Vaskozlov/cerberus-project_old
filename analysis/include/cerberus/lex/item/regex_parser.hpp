#ifndef CERBERUS_REGEX_PARSER_HPP
#define CERBERUS_REGEX_PARSER_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/lex/scan_api.hpp>
#include <cerberus/number.hpp>
#include <cerberus/reference_wrapper.hpp>

namespace cerb::lex::regex
{
    CERBERUS_EXCEPTION(RegexParsingError);

    template<CharacterLiteral CharT>
    struct RegexParser : scan::ScanApi<true, CharT>
    {
        constexpr static size_t number_of_chars = pow2<size_t>(bitsizeof(CharT));

        using bitmap_t = ConstBitmap<1, number_of_chars>;
        CERBLIB_SCAN_API_ACCESS(true, CharT);

        RegexParser() = default;

        constexpr RegexParser(GeneratorForText<CharT> &regex, bitmap_t &bitmap_for_chars)
          : scan_api_t(regex), available_chars(bitmap_for_chars)
        {
            parseRegex();
        }

    private:
        constexpr auto parseRegex() -> void
        {
            setupGenerator();
            checkRegexStart();

            while (canContinueParsing(cast(']'))) {
                CharT chr = getChar();
                processChar(chr);
            }
        }

        constexpr auto processChar(CharT chr) -> void
        {
            if (chr == cast('-')) {
                is_range_of_chars = true;
            } else {
                addCharToBitmap(chr);
            }
        }

        constexpr auto addCharToBitmap(CharT chr) -> void
        {
            if (is_range_of_chars) {
                fillRangeOfChars(previous_char, chr);
            } else {
                setChar(chr);
            }

            previous_char = chr;
        }

        constexpr auto fillRangeOfChars(CharT begin, CharT end) -> void
        {
            checkCharsOrder(begin, end);

            for (; begin != end; ++begin) {
                setChar(begin);
            }

            is_range_of_chars = false;
            previous_char = cast(0);
        }

        constexpr auto setChar(CharT chr) -> void
        {
            available_chars.template set<1, 0>(asUInt(chr));
        }

        CERBLIB_DECL static auto isBeginOfRegex(CharT chr) -> bool
        {
            return chr == cast('[');
        }

        constexpr static auto checkCharsOrder(CharT begin, CharT end) -> void
        {
            if (begin > end) {
                throw RegexParsingError("Chars in regex are in a wrong order!");
            }
        }

        constexpr auto checkRegexStart() -> void
        {
            if (not isBeginOfRegex(getChar())) {
                throw RegexParsingError("Unable to parse a regular expression");
            }
        }

        bitmap_t &available_chars;
        CharT previous_char{};
        bool is_range_of_chars{ false };
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct RegexParser<char>;
    extern template struct RegexParser<char8_t>;
    extern template struct RegexParser<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::lex::regex

#endif /* CERBERUS_REGEX_PARSER_HPP */
