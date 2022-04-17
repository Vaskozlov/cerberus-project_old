#ifndef CERBERUS_REGEX_PARSER_HPP
#define CERBERUS_REGEX_PARSER_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/number.hpp>
#include <cerberus/reference_wrapper.hpp>
#include <cerberus/string_view.hpp>
#include <cerberus/type.hpp>

namespace cerb::lex::regex
{
    CERBERUS_EXCEPTION(RegexParsingError);

    template<CharacterLiteral CharT>
    struct RegexParser
    {
        constexpr static size_t number_of_chars = pow2<size_t>(bitsizeof(CharT));
        using bitmap_t = ConstBitmap<1, number_of_chars>;

        template<std::integral T>
        static constexpr auto cast(T value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        RegexParser() = default;

        constexpr explicit RegexParser(GeneratorForText<CharT> &regex, bitmap_t &bitmap_for_chars)
          : regex_rule(regex), available_chars(bitmap_for_chars)
        {}

    private:
        constexpr auto parseRegex() -> void
        {
            checkInitialization();
            checkRegexStart();

            while (canContinueParsing()) {
                CharT chr = getChar();
                processChar(chr);
            }
        }

        constexpr auto processChar(CharT chr) -> void
        {
            switch (chr) {
            case cast('-'):

                break;

            case cast(']'):

                break;

            default:

                break;
            }
        }

        constexpr auto checkRegexStart() -> void
        {
            if (not isBeginOfRegex(getChar())) {
                throw RegexParsingError("Unable to parse a regular expression");
            }
        }

        constexpr auto checkInitialization() -> void
        {
            if (not regex_rule.isInitialized()) {
                regex_rule.getRawChar();
            }
        }

        CERBLIB_DECL auto canContinueParsing() const -> bool
        {
            CharT chr = getNextChar();

            if (isEoF(chr)) {
                throw RegexParsingError(
                    "End of regular expression reached, but it has not been closed!");
            }

            return chr != cast(']');
        }

        constexpr static auto isBeginOfRegex(CharT chr) -> bool
        {
            return chr == cast('[');
        }

        constexpr auto getChar() const -> CharT
        {
            return regex_rule.getCurrentChar();
        }

        constexpr auto nextChar() -> void
        {
            regex_rule.getCleanChar();
        }

        CERBLIB_DECL auto getNextChar() -> CharT
        {
            nextChar();
            return getChar();
        }

        GeneratorForText<CharT> &regex_rule;
        bitmap_t &available_chars;
        bool is_range_of_chars{ false };
    };
}// namespace cerb::lex::regex

#endif /* CERBERUS_REGEX_PARSER_HPP */
