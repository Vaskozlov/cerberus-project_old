#ifndef CERBERUS_REGEX_PARSER_HPP
#define CERBERUS_REGEX_PARSER_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/lex/item/errros/regex_parsing_errors.hpp>
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

        friend RegexParsingErrors<CharT>;

        using scan_api_t::cast;
        using Error = RegexParsingErrors<CharT>;

        constexpr RegexParser(text::GeneratorForText<CharT> &regex, Bitmap &bitmap_for_chars)
          : scan_api_t(regex), available_chars(bitmap_for_chars)
        {
            scan_api_t::beginScanning(']');
        }

    private:
        constexpr auto onStart() -> text::ScanApiStatus override
        {
            Error::checkRegexStart(*this);
            return text::ScanApiStatus::SKIP_CHAR;
        }

        constexpr auto processChar(CharT chr) -> void override
        {
            Error::checkDoubleRegexOpening(*this, chr);

            if (chr == cast('-')) {
                is_range_of_chars = true;
            } else {
                chr = processEscapeSymbol(chr);
                addCharToBitmap(chr);
            }
        }

        constexpr auto onEnd() -> void override
        {
            Error::checkRangeClosing(*this);
            Error::checkRangeNonEmpty(*this);
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
            Error::checkCharsOrder(*this, begin, end);
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
            available_chars.template set<1>(asUInt(chr));
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
