#ifndef CERBERUS_REGEX_PARSING_ERRORS_HPP
#define CERBERUS_REGEX_PARSING_ERRORS_HPP

#include <cerberus/lex/item/basic_item.hpp>

namespace cerb::lex::regex
{
    template<CharacterLiteral CharT>
    struct RegexParser;

    CERBERUS_EXCEPTION(BasicRegexParsingError, BasicLexicalAnalysisException);

    template<CharacterLiteral CharT>
    CERBERUS_ANALYSIS_EXCEPTION(RegexParsingError, CharT, BasicRegexParsingError);

    template<CharacterLiteral CharT>
    struct RegexParsingChecks
    {
        using regex_parser_t = RegexParser<CharT>;

        constexpr static auto charsOrder(regex_parser_t const &regex_parser, CharT begin, CharT end)
            -> void
        {
            if (begin > end) {
                throwException("Chars in regex are in a wrong order!", regex_parser);
            }
        }

        constexpr static auto regexStart(regex_parser_t const &regex_parser) -> void
        {
            if (not isBeginOfRegex(regex_parser.getChar())) {
                throwException("Unable to parse a regular expression!", regex_parser);
            }
        }

        constexpr static auto rangeClosing(regex_parser_t const &regex_parser) -> void
        {
            if (regex_parser.is_range_of_chars) {
                throwException("Range of chars is not closed!", regex_parser);
            }
        }

        constexpr static auto rangeNonEmpty(regex_parser_t const &regex_parser) -> void
        {
            if (not regex_parser.is_filled) {
                throwException("There are not any characters in regex!", regex_parser);
            }
        }

        constexpr static auto doubleRegexOpening(regex_parser_t const &regex_parser, CharT chr)
            -> void
        {
            if (chr == static_cast<CharT>('[')) {
                throwException(
                    "Unable to open regex inside regex. If you want to use "
                    "'[' as a character type use '\\[' instead.",
                    regex_parser);
            }
        }

    private:
        CERBLIB_DECL static auto isBeginOfRegex(CharT chr) -> bool
        {
            return chr == static_cast<CharT>('[');
        }

        constexpr static auto
            throwException(string_view const &message, regex_parser_t const &regex_parser) -> void
        {
            throw RegexParsingError(message, regex_parser.getGenerator());
        }
    };
}// namespace cerb::lex::regex

#endif /* CERBERUS_REGEX_PARSING_ERRORS_HPP */
