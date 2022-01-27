#ifndef CERBERUS_STRING_PARSER_HPP
#define CERBERUS_STRING_PARSER_HPP

#include <cerberus/flat_map.hpp>
#include <cerberus/lexical/char.hpp>
#include <cerberus/lexical/lexical_exceptions.hpp>
#include <cerberus/lexical/file_location.hpp>
#include <cerberus/string_view.hpp>
#include <string>

namespace cerb::lex
{
    template<CharacterLiteral CharT, Iterable T>
    class StringParser
    {
        using chars_enum = CharsEnum<CharT>;
        using str_t = std::basic_string<CharT>;

        template<std::integral U>
        CERBLIB_DECL static auto cast(U value) -> CharT
        {
            return static_cast<CharT>(value);
        }

    public:
        CERBLIB_DECL auto get() -> str_t &
        {
            return parsed_string;
        }

        CERBLIB_DECL auto get() const -> str_t &
        {
            return parsed_string;
        }

        CERBLIB_DECL auto getIterator() -> decltype(auto)
        {
            return iterator_for_value;
        }

        CERBLIB_DECL auto getIterator() const -> decltype(auto)
        {
            return iterator_for_value;
        }

        constexpr auto parseString() -> str_t &
        {
            if (*iterator_for_value != chars_enum::DQM) {
                throw ParsingError("Given input is not a string!");
            }

            incAndCheckThatStringDoesNotEnd();

            for (; iterator_for_value != iterable.end(); ++iterator_for_value) {
                auto chr = *iterator_for_value;

                if (chr == string_char) {
                    return parsed_string;
                }

                switch (chr) {
                case chars_enum::EoF:
                    throw ParsingError("Unable to find end of string.");

                case chars_enum::Backslash:
                    parseSpecialSymbol();
                    break;

                default:
                    parsed_string.push_back(chr);
                    break;
                }
            }

            throw ParsingError("Unable to find end of string.");
        }

        constexpr auto init(CharT string_starter, T &iterable_value) -> void
        {
            iterable = iterable_value;
            string_char = string_starter;
        }

        constexpr StringParser() = default;
        constexpr StringParser(CharT string_starter, T &iterable_value)
          : iterable(iterable_value), iterator_for_value(iterable_value.begin()),
            string_char(string_starter)
        {}

    private:
        constexpr auto parseSpecialSymbol() -> void
        {
            incAndCheckThatStringDoesNotEnd();
            auto chr = *iterator_for_value;

            switch (chr) {
            case cast('t'):
                parsed_string.push_back(chars_enum::Tab);
                break;

            case 'n':
                parsed_string.push_back(chars_enum::NewLine);
                break;

            case cast('r'):
                parsed_string.push_back(chars_enum::CarriageReturn);
                break;

            case cast('\''):
                parsed_string.push_back(chars_enum::Apostrophe);
                break;

            case cast('\"'):
                parsed_string.push_back(chars_enum::DQM);
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
                throw ParsingError("Unable to match any special symbol");
            }
        }

        CERBLIB_DECL auto parseOctalSymbol(size_t length) -> CharT
        {
            return convertString2Int(8, length);
        }

        CERBLIB_DECL auto parseHexSymbol(size_t length) -> CharT
        {
            return convertString2Int(16, length);
        }

        CERBLIB_DECL auto convertString2Int(size_t notation, size_t length) -> CharT
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
            if (not hexadecimal_chars.contains(*iterator_for_value)) {
                throw std::invalid_argument("Unable to create character by it's value");
            }

            auto number = hexadecimal_chars[*iterator_for_value];

            if (number >= cast(notation)) {
                throw std::range_error("Unable to create character by it's value");
            }

            return cast(number);
        }

        constexpr auto incAndCheckThatStringDoesNotEnd() -> void
        {
            ++iterator_for_value;

            if (iterator_for_value == iterable.end()) {
                throw ParsingError(
                    "End of string reached, however special symbol for this have not been found");
            }
        }

        static constexpr auto hexadecimal_chars = HexadecimalCharsToInt<CharT>;

        str_t parsed_string{};
        T &iterable;
        decltype(std::begin(iterable)) iterator_for_value;
        CharT string_char{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_STRING_PARSER_HPP */
