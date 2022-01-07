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
        using str_view = BasicStringView<CharT>;
        using text_iterator = GetIteratorType<TextGenerator<CharT>>;

        constexpr auto reverse() -> void
        {
            available_chars.template reverseBits<0>();
        }

        CERBLIB_DECL auto getIterator() const -> text_iterator
        {
            return iterator_for_text;
        }

        constexpr SequenceParser() = default;
        constexpr SequenceParser(text_iterator begin, text_iterator end)
          : iterator_for_text(begin), end_of_text(end)
        {
            parseSequence();
        }

    private:
        template<std::integral T>
        CERBLIB_DECL static auto cast(T value)
        {
            return static_cast<CharT>(value);
        }

        constexpr auto parseSequence() -> void
        {
            if (*iterator_for_text != '[') {
                throw DotItemNotASequenceError("Unable to match '[' to start sequence");
            }

            ++iterator_for_text;
            parseCharsInSequence();
        }

        constexpr auto parseCharsInSequence() -> void
        {
            bool need_to_fill_range = false;
            CharT previous_char = cast('\0');

            for (; iterator_for_text != end_of_text; ++iterator_for_text) {
                if (canStartRange()) {
                    need_to_fill_range = true;
                    continue;
                }
                if (*iterator_for_text == cast('-')) {
                    ++iterator_for_text;

                    if (iterator_for_text == end_of_text) {
                        break;
                    }
                }

                if (skipCharsOrShouldStop()) {
                    return;
                }

                if (need_to_fill_range) {
                    fillRange(previous_char, *iterator_for_text);
                } else {
                    setChar(*iterator_for_text);
                }

                previous_char = *iterator_for_text;
                need_to_fill_range = false;
            }

            throw DotItemNotASequenceError("Unable to close sequence");
        }

        CERBLIB_DECL auto canStartRange() const -> bool
        {
            return logicalAnd(
                *iterator_for_text == cast('-'), iterator_for_text.nextChar() != cast('-'));
        }

        template<CharacterLiteral T>
        CERBLIB_DECL auto charRepeatTwice(T chr) const -> bool
        {
            return *iterator_for_text == cast(chr) && iterator_for_text.nextChar() == chr;
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
            if (charRepeatTwice('[')) {
                ++iterator_for_text;
            } else {
                throw DotItemNotASequenceError(
                    "Unable to open sequence inside other sequence. If you mean '[' as a "
                    "character use '[[' instead");
            }
        }

        CERBLIB_DECL auto skipCharIfNotEndOfSequence() -> bool
        {
            if (charRepeatTwice(']')) {
                ++iterator_for_text;
                return false;
            }
            return true;
        }

        template<std::integral T>
        constexpr auto setChar(T chr) -> void
        {
            available_chars.template set<1, 0>(convert2UnsignedInt(chr));
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

        constexpr static size_t number_of_chars = 1ULL << bitsizeof(CharT);

        ConstBitmap<1, number_of_chars> available_chars{};
        text_iterator iterator_for_text{};
        text_iterator end_of_text{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_SEQUENCE_PARSER_HPP */
