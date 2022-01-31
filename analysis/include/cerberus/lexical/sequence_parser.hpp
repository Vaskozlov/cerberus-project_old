#ifndef CERBERUS_SEQUENCE_PARSER_HPP
#define CERBERUS_SEQUENCE_PARSER_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/lexical/char.hpp>
#include <cerberus/lexical/file_location.hpp>
#include <cerberus/lexical/lexical_exceptions.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct SequenceParser
    {
        constexpr static size_t number_of_chars = pow2<size_t>(bitsizeof(CharT));

        using chars_enum_t = CharsEnum<CharT>;
        using generator_t = TextGenerator<CharT>;
        using str_view_t = BasicStringView<CharT>;
        using constant_bitmap_t = ConstBitmap<1, number_of_chars>;

        constexpr auto reverse() -> void
        {
            available_chars.template reverseBits<0>();
        }

        constexpr SequenceParser(constant_bitmap_t &bitmap, ReferenceWrapper<generator_t> generator)
          : available_chars(bitmap), text_generator(generator.get())
        {
            parseCharsInSequence();
        }

    private:
        template<std::integral T>
        CERBLIB_DECL static auto cast(T value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        constexpr auto incAndCheckThatStringDoesNotEnd() -> void
        {
            if (text_generator.getCharWithoutLayout() == chars_enum_t::EoF) {
                throw SequenceError("Unable to close sequence");
            }
        }

        constexpr auto parseCharsInSequence() -> void
        {
            bool is_range_of_chars = false;
            CharT previous_char = chars_enum_t::EoF;
            CharT chr = text_generator.getCurrentChar();

            if (chr != cast('[')) {
                throw SequenceError("Unable to find '[' to start sequence!");
            }

            while ((chr = text_generator.getCharWithoutLayout()) != chars_enum_t::EoF) {
                if (tryToStartRangeOrSkipExtraStartSymbol()) {
                    is_range_of_chars = true;
                    continue;
                }

                if (skipCharsOrShouldStop()) {
                    return;
                }

                fillAccordingToRule(is_range_of_chars, previous_char);

                previous_char = chr;
                is_range_of_chars = false;
            }

            throw SequenceError("Unable to close sequence");
        }

        constexpr auto fillAccordingToRule(bool is_range_of_chars, CharT previous_char) -> void
        {
            if (is_range_of_chars) {
                fillRange(previous_char, text_generator.getCurrentChar());
            } else {
                setChar(text_generator.getCurrentChar());
            }
        }

        constexpr auto fillRange(CharT first, CharT last) -> void
        {
            if (first > last) {
                throw SequenceError("DotItem::SequenceOfChar error! Range start with lower char!");
            }

            auto converted_first = static_cast<i32>(first);
            auto converted_last = static_cast<i32>(last);

            for (; converted_first != converted_last; ++converted_first) {
                setChar(converted_first);
            }
        }

        template<std::integral T>
        constexpr auto setChar(T chr) -> void
        {
            available_chars.template set<1, 0>(convert2UnsignedInt(chr));
        }

        CERBLIB_DECL auto tryToStartRangeOrSkipExtraStartSymbol() -> bool
        {
            if (text_generator.getCurrentChar() == cast('-')) {
                return skipExtraSymbolOrAllowToStartRange();
            }

            return false;
        }

        CERBLIB_DECL auto skipExtraSymbolOrAllowToStartRange() -> bool
        {
            if (text_generator.getCurrentChar(1) == cast('-')) {
                incAndCheckThatStringDoesNotEnd();
                return false;
            }

            return true;
        }

        CERBLIB_DECL auto skipCharsOrShouldStop() -> bool
        {
            switch (text_generator.getCurrentChar()) {
            case cast('['):
                checkFor2OpenedSequences();
                break;

            case cast(']'):
                return skipCharIfNotEndOfSequence();

            default:
                break;
            }

            return false;
        }

        constexpr auto checkFor2OpenedSequences() -> void
        {
            if (doesCharRepeatTwice('[')) {
                incAndCheckThatStringDoesNotEnd();
            } else {
                throw SequenceError(
                    "Unable to open sequence inside other sequence. If you mean '[' as a "
                    "character use '[[' instead");
            }
        }

        CERBLIB_DECL auto skipCharIfNotEndOfSequence() -> bool
        {
            if (doesCharRepeatTwice(']')) {
                incAndCheckThatStringDoesNotEnd();
                return false;
            }
            return true;
        }

        template<CharacterLiteral T>
        CERBLIB_DECL auto doesCharRepeatTwice(T chr) const -> bool
        {
            return logicalAnd(
                text_generator.getCurrentChar() == cast(chr),
                text_generator.getCurrentChar(1) == cast(chr));
        }

        constant_bitmap_t &available_chars;
        generator_t &text_generator;
    };
}// namespace cerb::lex

#endif /* CERBERUS_SEQUENCE_PARSER_HPP */
