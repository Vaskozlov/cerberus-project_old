#ifndef CERBERUS_REGEX_PARSER_HPP
#define CERBERUS_REGEX_PARSER_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/lex/lexical_analysis_exception.hpp>
#include <cerberus/number.hpp>
#include <cerberus/reference_wrapper.hpp>
#include <cerberus/text/scan_api.hpp>

namespace cerb::lex::regex
{
    template<CharacterLiteral CharT>
    struct RegexParser : text::ScanApi<false, CharT>
    {
        CERBLIB_SCAN_API_ACCESS(false, CharT);
        CERBERUS_ANALYSIS_EXCEPTION(RegexParsingError, CharT, BasicLexicalAnalysisException);

        using scan_api_t::cast;

        constexpr RegexParser(text::GeneratorForText<CharT> &regex, Bitmap &bitmap_for_chars)
          : scan_api_t(regex), available_chars(bitmap_for_chars)
        {
            scan_api_t::beginScanning(']');
        }

    private:
        constexpr auto onStart() -> text::ScanApiStatus override
        {
            checkRegexStart();
            return text::ScanApiStatus::SKIP_CHAR;
        }

        constexpr auto processChar(CharT chr) -> void override
        {
            checkDoubleRegexOpening(chr);

            if (chr == cast('-')) {
                is_range_of_chars = true;
            } else {
                chr = processEscapeSymbol(chr);
                addCharToBitmap(chr);
            }
        }

        constexpr auto onEnd() -> void override
        {
            checkRangeClosing();
            checkRangeNonEmpty();
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
            available_chars.multiSet<1>(asUInt(begin), asUInt(end) + 1);

            is_range_of_chars = false;
            previous_char = CharEnum<CharT>::EoF;
        }

        constexpr auto processEscapeSymbol(CharT chr) -> CharT
        {
            if (chr == cast('\\')) {
                return parseEscapeSequence(
                    { { '\\', '\\' }, { '-', '-' }, { '[', '[' }, { ']', ']' } });
            }
            return chr;
        }

        constexpr auto setChar(CharT chr) -> void
        {
            is_filled = true;
            available_chars.set<1>(asUInt(chr));
        }

        CERBLIB_DECL static auto isBeginOfRegex(CharT chr) -> bool
        {
            return chr == cast('[');
        }

        constexpr auto checkCharsOrder(CharT begin, CharT end) const -> void
        {
            if (begin > end) {
                throw RegexParsingError("Chars in regex are in a wrong order!", getGenerator());
            }
        }

        constexpr auto checkRegexStart() const -> void
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
                throw RegexParsingError("There are not any characters in regex!", getGenerator());
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

        Bitmap &available_chars;
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
