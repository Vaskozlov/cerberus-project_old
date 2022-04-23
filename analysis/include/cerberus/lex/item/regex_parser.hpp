#ifndef CERBERUS_REGEX_PARSER_HPP
#define CERBERUS_REGEX_PARSER_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/exception.hpp>
#include <cerberus/lex/generator_for_text.hpp>
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
        {
            parseRegex();
        }

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

        constexpr auto checkInitialization() -> void
        {
            if (not regex_rule.isInitialized()) {
                regex_rule.getRawChar();
            }
        }

        CERBLIB_DECL auto canContinueParsing() -> bool
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
        CharT previous_char{};
        bool is_range_of_chars{ false };
    };
}// namespace cerb::lex::regex

#endif /* CERBERUS_REGEX_PARSER_HPP */
