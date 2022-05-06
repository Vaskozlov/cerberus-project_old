#ifndef CERBERUS_REGEX_PARSER_HPP
#define CERBERUS_REGEX_PARSER_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/lex/scan_api.hpp>
#include <cerberus/number.hpp>
#include <cerberus/reference_wrapper.hpp>

namespace cerb::lex::regex
{
    template<CharacterLiteral CharT>
    struct RegexParser : scan::ScanApi<false, CharT>
    {
        CERBLIB_SCAN_API_ACCESS(false, CharT);
        CERBERUS_ANALYSIS_EXCEPTION(RegexParsingError, CharT, BasicLexicalAnalysisException);

        constexpr static size_t number_of_chars = pow2<size_t>(bitsizeof(CharT));
        using bitmap_t = ConstBitmap<1, number_of_chars>;

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

            checkRangeClosing();
            checkRangeNonEmpty();
        }

        constexpr auto processChar(CharT chr) -> void
        {
            checkDoubleRegexOpening(chr);

            if (chr == cast('-')) {
                is_range_of_chars = true;
            } else {
                chr = processEscapeSymbol(chr);
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

            for (; begin <= end; ++begin) {
                setChar(begin);
            }

            is_range_of_chars = false;
            previous_char = cast(0);
        }

        constexpr auto processEscapeSymbol(CharT chr) -> CharT
        {
            if (chr == cast('\\')) {
                return parseEscapeSequence(chr, cast('-'), cast('['), cast(']'));
            } else {
                return chr;
            }
        }

        constexpr auto setChar(CharT chr) -> void
        {
            is_filled = true;
            available_chars.template set<1, 0>(asUInt(chr));
        }

        CERBLIB_DECL static auto isBeginOfRegex(CharT chr) -> bool
        {
            return chr == cast('[');
        }

        constexpr auto checkCharsOrder(CharT begin, CharT end) -> void
        {
            if (begin > end) {
                throw RegexParsingError("Chars in regex are in a wrong order!", getGenerator());
            }
        }

        constexpr auto checkRegexStart() -> void
        {
            if (not isBeginOfRegex(getChar())) {
                throw RegexParsingError("Unable to parse a regular expression!", getGenerator());
            }
        }

        constexpr auto checkRangeClosing() const -> void
        {
            if (is_range_of_chars) {
                throw RegexParsingError("Range of chars is not closed!", getGenerator());
            }
        }

        constexpr auto checkRangeNonEmpty() const -> void
        {
            if (not is_filled) {
                throw RegexParsingError("There are no characters in regex!", getGenerator());
            }
        }

        constexpr auto checkDoubleRegexOpening(CharT chr) const -> void
        {
            if (chr == cast('[')) {
                throw RegexParsingError(
                    "Unable to open regex inside regex. If you want to use "
                    "'[' as a character type use '\\[' instead.",
                    getGenerator());
            }
        }

        bitmap_t &available_chars;
        CharT previous_char{};
        bool is_filled{ false };
        bool is_range_of_chars{ false };
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct RegexParser<char>;
    extern template struct RegexParser<char8_t>;
    extern template struct RegexParser<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::lex::regex

#endif /* CERBERUS_REGEX_PARSER_HPP */
