#ifndef CERBERUS_STRING_TO_CODES_HPP
#define CERBERUS_STRING_TO_CODES_HPP

#include <cerberus/exception.hpp>
#include <cerberus/lex/char.hpp>
#include <cerberus/lex/generator_for_text.hpp>
#include <string>

namespace cerb::lex
{
    CERBERUS_EXCEPTION(StringToCodesTranslationError);

    template<CharacterLiteral CharT>
    class StringToCodes
    {
        CERBLIB_DECL auto getChar() const -> CharT
        {
            return parsing_text.getCurrentChar();
        }

        CERBLIB_DECL auto getFutureChar() const -> CharT
        {
            return parsing_text.getCurrentChar(1);
        }

        constexpr auto nextChar() -> void
        {
            parsing_text.getRawChar();
        }

        CERBLIB_DECL auto getNextChar() -> CharT
        {
            nextChar();
            return getChar();
        }

        CERBLIB_DECL auto getNextCharAndCheckForEoF() -> CharT
        {
            auto chr = getNextChar();

            if (isEoF(chr)) {
                throw StringToCodesTranslationError("Unexpected EoF");
            }

            return chr;
        }

        CERBLIB_DECL auto isBeginOfString(CharT chr) const -> bool
        {
            return chr == string_begin_char;
        }

        template<std::integral T>
        static constexpr auto cast(T value) -> CharT
        {
            return static_cast<CharT>(value);
        }

    public:
        constexpr auto parseString() -> std::basic_string<CharT> &
        {
            checkInitialization();
            checkStringStart();

            while (canContinueParsing()) {
                CharT chr = getChar();
                processChar(chr);
            }

            return parsed_string;
        }

        StringToCodes() = default;

        constexpr StringToCodes(CharT string_opener, GeneratorForText<CharT> &text)
          : parsing_text(text), string_begin_char(string_opener)
        {}

    private:
        constexpr auto processChar(CharT chr) -> void
        {
            if (chr == CharEnum<CharT>::Backlash) {
                parseEscapeSequence();
            } else {
                parsed_string.push_back(chr);
            }
        }

        constexpr auto parseEscapeSequence() -> void
        {
            CharT chr = getNextCharAndCheckForEoF();

            switch (chr) {
            case cast('\\'):
                parsed_string.push_back('\\');
                break;

            case cast('\''):
                parsed_string.push_back('\'');
                break;

            case cast('\"'):
                parsed_string.push_back('\"');
                break;

            case cast('t'):
                parsed_string.push_back('\t');
                break;

            case cast('n'):
                parsed_string.push_back('\n');
                break;

            case cast('r'):
                parsed_string.push_back('\r');
                break;

            case cast('f'):
                parsed_string.push_back('\f');
                break;

            case cast('b'):
                parsed_string.push_back('\b');
                break;

            case cast('0'):
                parsed_string.push_back(convertStringToChar(8, 2));
                break;

            case cast('x'):
                parsed_string.push_back(convertStringToChar(16, 2));
                break;

            case cast('u'):
                parsed_string.push_back(convertStringToChar(16, 4));
                break;

            default:
                throw StringToCodesTranslationError("Unable to match any escape sequence");
            }
        }

        CERBLIB_DECL auto convertStringToChar(u32 notation, u32 length) -> CharT
        {
            CharT resulted_char = CharEnum<CharT>::EoF;

            for (CERBLIB_UNUSED(u32) : Range(length)) {
                if (doesNotFitIntoNotation(getFutureChar(), notation)) {
                    break;
                }

                resulted_char *= cast(notation);
                resulted_char += convertSymbolToInt(getNextCharAndCheckForEoF());
            }

            return resulted_char;
        }

        CERBLIB_DECL static auto convertSymbolToInt(CharT chr) -> CharT
        {
            return cast(hexadecimal_chars[chr]);
        }

        constexpr auto canContinueParsing() -> bool
        {
            CharT chr = getNextChar();

            if (isEoF(chr)) {
                throw StringToCodesTranslationError("Unable to close string!");
            }

            return chr != cast(string_begin_char);
        }

        constexpr auto checkStringStart() const -> void
        {
            if (not isBeginOfString(getChar())) {
                throw StringToCodesTranslationError("Unable to open string!");
            }
        }

        constexpr auto checkInitialization() -> void
        {
            if (not parsing_text.isInitialized()) {
                nextChar();
            }
        }

        CERBLIB_DECL static auto doesNotFitIntoNotation(CharT chr, u32 notation) -> bool
        {
            if (not hexadecimal_chars.contains(chr)) {
                return true;
            }

            auto code = convertSymbolToInt(chr);
            return static_cast<i32>(code) >= static_cast<i32>(notation);
        }

        static constexpr auto hexadecimal_chars = HexadecimalCharsToInt<CharT>;

        std::basic_string<CharT> parsed_string{};
        GeneratorForText<CharT> &parsing_text;
        CharT string_begin_char{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_STRING_TO_CODES_HPP */
