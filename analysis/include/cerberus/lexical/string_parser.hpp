#ifndef CERBERUS_STRING_PARSER_HPP
#define CERBERUS_STRING_PARSER_HPP

#include <cerberus/flat_map.hpp>
#include <cerberus/lexical/char.hpp>
#include <cerberus/lexical/file_location.hpp>
#include <cerberus/lexical/lexical_exceptions.hpp>
#include <cerberus/lexical/text_generator.hpp>
#include <cerberus/string_view.hpp>
#include <string>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    class StringParser
    {
        using chars_enum_t = CharsEnum<CharT>;
        using str_t = std::basic_string<CharT>;
        using generator_t = TextGenerator<CharT>;

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

        constexpr auto parseString() -> str_t &
        {
            if (generator.getCurrentChar() != string_char) {
                throw ParsingError("Given input is not a string!");
            }

            while (not isEoF(generator.getRawChar())) {
                CharT chr = generator.getCurrentChar();

                if (chr == string_char) {
                    return parsed_string;
                }

                switch (chr) {
                case chars_enum_t::EoF:
                    throw ParsingError("Unable to find end of string.");

                case chars_enum_t::Backslash:
                    parseSpecialSymbol();
                    break;

                default:
                    parsed_string.push_back(chr);
                    break;
                }
            }

            throw ParsingError("Unable to find end of string.");
        }

        constexpr auto reinit(CharT string_starter, generator_t &text_generator) -> void
        {
            generator = text_generator;
            string_char = string_starter;
        }

        constexpr StringParser(CharT string_starter, generator_t &text_generator)
          : generator(text_generator), string_char(string_starter)
        {}

    private:
        constexpr auto parseSpecialSymbol() -> void
        {
            incAndCheckThatStringDoesNotEnd();
            auto chr = generator.getCurrentChar();

            switch (chr) {
            case cast('t'):
                parsed_string.push_back(chars_enum_t::Tab);
                break;

            case 'n':
                parsed_string.push_back(chars_enum_t::NewLine);
                break;

            case cast('r'):
                parsed_string.push_back(chars_enum_t::CarriageReturn);
                break;

            case cast('\''):
                parsed_string.push_back(chars_enum_t::Apostrophe);
                break;

            case cast('\"'):
                parsed_string.push_back(chars_enum_t::DQM);
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
            CharT resulted_char = chars_enum_t::EoF;

            for (size_t i = 0; i < length; ++i) {
                incAndCheckThatStringDoesNotEnd();
                resulted_char *= cast(notation);
                resulted_char += tryToConvertCharToInt(notation);
            }

            return resulted_char;
        }

        CERBLIB_DECL auto tryToConvertCharToInt(size_t notation) -> CharT
        {
            if (not hexadecimal_chars.contains(generator.getCurrentChar())) {
                throw std::invalid_argument("Unable to create character by it's value");
            }

            auto number = hexadecimal_chars[generator.getCurrentChar()];

            if (number >= cast(notation)) {
                throw std::range_error("Unable to create character by it's value");
            }

            return cast(number);
        }

        constexpr auto incAndCheckThatStringDoesNotEnd() -> void
        {
            if (isEoF(generator.getRawChar())) {
                throw ParsingError(
                    "End of string reached, however special symbol for this have not been found");
            }
        }

        static constexpr auto hexadecimal_chars = HexadecimalCharsToInt<CharT>;

        str_t parsed_string{};
        generator_t &generator;
        CharT string_char{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_STRING_PARSER_HPP */
