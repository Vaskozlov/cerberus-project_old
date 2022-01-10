#ifndef CERBERUS_STRING_PARSER_HPP
#define CERBERUS_STRING_PARSER_HPP

#include <cerberus/flat_map.hpp>
#include <cerberus/lexical/lexical_exceptions.hpp>
#include <cerberus/lexical/location.hpp>
#include <cerberus/string_view.hpp>
#include <string>

namespace cerb::lex {
    template<CharacterLiteral CharT, typename Iterator>
    class StringParser
    {
        template<std::integral T>
        CERBLIB_DECL static auto cast(T value)
        {
            return static_cast<CharT>(value);
        }

    public:
        CERBLIB_DECL auto get() -> std::basic_string<CharT> &
        {
            return parsed_string;
        }

        CERBLIB_DECL auto get() const -> std::basic_string<CharT> &
        {
            return parsed_string;
        }

        CERBLIB_DECL auto getIterator() -> Iterator
        {
            return begin_of_string;
        }

        CERBLIB_DECL auto getIterator() const -> Iterator
        {
            return begin_of_string;
        }

        constexpr auto parseString() -> std::basic_string<CharT> &
        {
            if (*begin_of_string != string_char) {
                throw StringParsingError("Unable to parse a string, because it's not a string!.");
            }

            incAndCheckThatStringDoesNotEnd();

            for (; begin_of_string != end_of_string; ++begin_of_string) {
                auto chr = *begin_of_string;

                if (chr == string_char) {
                    ++begin_of_string;
                    return parsed_string;
                }

                switch (chr) {
                case '\0':
                    throw StringParsingError("Unable to find end of string.");

                case '\\':
                    parseSpecialSymbol();
                    break;

                default:
                    parsed_string.push_back(chr);
                    break;
                }
            }

            throw StringParsingError("Unable to find end of string.");
        }

        constexpr auto init(CharT string_starter, Iterator &begin, Iterator const &end) -> void
        {
            begin_of_string = begin;
            end_of_string = end;
            string_char = string_starter;
        }

        constexpr StringParser() = default;
        constexpr StringParser(CharT string_starter, Iterator &begin, Iterator const &end)
          : begin_of_string(begin), end_of_string(end), string_char(string_starter)
        {}

    private:
        constexpr auto parseSpecialSymbol() -> void
        {
            incAndCheckThatStringDoesNotEnd();
            auto chr = *begin_of_string;

            switch (chr) {
            case cast('t'):
                parsed_string.push_back(cast('\t'));
                break;

            case 'n':
                parsed_string.push_back(cast('\n'));
                break;

            case cast('r'):
                parsed_string.push_back('\r');
                break;

            case cast('\''):
                parsed_string.push_back('\'');
                break;

            case cast('\"'):
                parsed_string.push_back('\"');
                break;

            case cast('0'):
                parsed_string.push_back(parseOctalSymbol(2));
                break;

            case cast('x'):
                parsed_string.push_back(parseHexSymbol(2));
                break;

            case cast('u'):
                parsed_string.push_back(parseHexSymbol(4));
                break;

            default:
                throw StringParsingError("Unable to match any special symbol");
            }
        }

        constexpr auto parseOctalSymbol(size_t length) -> CharT
        {
            return convertString2Int(8, length);
        }

        constexpr auto parseHexSymbol(size_t length) -> CharT
        {
            return convertString2Int(16, length);
        }

        constexpr auto convertString2Int(size_t notation, size_t length) -> CharT
        {
            CharT resulted_char = cast(0);

            for (size_t i = 0; i < length; ++i) {
                incAndCheckThatStringDoesNotEnd();
                resulted_char *= cast(notation);
                resulted_char += tryToConvertCharToInt(notation);
            }

            return resulted_char;
        }

        constexpr auto tryToConvertCharToInt(size_t notation) -> CharT
        {
            if (!CharsToInt<CharT>.contains(*begin_of_string)) {
                throw std::invalid_argument("Unable to create character by it's value");
            }

            auto number = CharsToInt<CharT>[*begin_of_string];

            if (number >= cast(notation)) {
                throw std::range_error("Unable to create character by it's value");
            }

            return cast(number);
        }

        constexpr auto incAndCheckThatStringDoesNotEnd() -> void
        {
            ++begin_of_string;

            if (begin_of_string == end_of_string) {
                throw StringParsingError(
                    "End of string reached, however special symbol for this have not been found");
            }
        }

        std::basic_string<CharT> parsed_string{};
        Iterator &begin_of_string{};
        Iterator const &end_of_string{};
        CharT string_char{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_STRING_PARSER_HPP */
