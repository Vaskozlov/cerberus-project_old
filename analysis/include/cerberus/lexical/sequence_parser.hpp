#ifndef CERBERUS_SEQUENCE_PARSER_HPP
#define CERBERUS_SEQUENCE_PARSER_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/lexical/char.hpp>
#include <cerberus/lexical/lexical_exceptions.hpp>
#include <cerberus/lexical/location.hpp>

namespace cerb::lex {
    template<CharacterLiteral CharT>
    struct SequenceParser
    {
        constexpr static size_t number_of_chars = 1ULL << bitsizeof(CharT);

        using str_view = BasicStringView<CharT>;
        using constant_bitmap = ConstBitmap<1, number_of_chars>;
        using text_iterator = GetIteratorType<GeneratorForText<CharT>>;

        constexpr auto reverse() -> void
        {
            available_chars.template reverseBits<0>();
        }

        constexpr SequenceParser(
            constant_bitmap &bitmap,
            text_iterator &begin,
            text_iterator const &end)
          : available_chars(bitmap), iterator_for_text(begin), end_of_text(end)
        {
            parseSequence();
        }

    private:
        template<std::integral T>
        CERBLIB_DECL static auto cast(T value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        constexpr auto incAndCheckThatStringDoesNotEnd() -> void
        {
            ++iterator_for_text;
            if (iterator_for_text == end_of_text) {
                throw DotItemNotASequenceError("Unable to close sequence");
            }
        }

        constexpr auto parseSequence() -> void
        {
            if (*iterator_for_text != '[') {
                throw DotItemNotASequenceError("Unable to match '[' to start sequence");
            }

            incAndCheckThatStringDoesNotEnd();
            parseCharsInSequence();
        }

        constexpr auto parseCharsInSequence() -> void
        {
            bool is_range_of_chars = false;
            CharT previous_char = cast('\0');

            for (; iterator_for_text != end_of_text; ++iterator_for_text) {
                if (canStartRange()) {
                    is_range_of_chars = true;
                    continue;
                }

                if (*iterator_for_text == cast('-')) {
                    incAndCheckThatStringDoesNotEnd();
                }

                if (skipCharsOrShouldStop()) {
                    return;
                }

                fillAccordingToRule(is_range_of_chars, previous_char);

                previous_char = *iterator_for_text;
                is_range_of_chars = false;
            }

            throw DotItemNotASequenceError("Unable to close sequence");
        }

        constexpr auto fillAccordingToRule(bool is_range_of_chars, CharT previous_char) -> void
        {
            if (is_range_of_chars) {
                fillRange(previous_char, *iterator_for_text);
            } else {
                setChar(*iterator_for_text);
            }
        }

        constexpr auto fillRange(CharT first, CharT last) -> void
        {
            if (first > last) {
                throw DotItemNotASequenceError(
                    "DotItem::SequenceOfChar error! Range start with lower char!");
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

        CERBLIB_DECL auto canStartRange() const -> bool
        {
            return logicalAnd(
                *iterator_for_text == cast('-'),
                iterator_for_text.getCharAtCurrentOffset(1) != cast('-'));
        }

        CERBLIB_DECL auto skipCharsOrShouldStop() -> bool
        {
            switch (*iterator_for_text) {
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
                throw DotItemNotASequenceError(
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
                *iterator_for_text == cast(chr),
                iterator_for_text.getCharAtCurrentOffset(1) == chr);
        }

        constant_bitmap &available_chars{};
        text_iterator &iterator_for_text{};
        text_iterator const &end_of_text{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_SEQUENCE_PARSER_HPP */
