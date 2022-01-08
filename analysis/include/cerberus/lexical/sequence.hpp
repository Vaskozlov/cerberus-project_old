#ifndef CERBERUS_SEQUENCE_HPP
#define CERBERUS_SEQUENCE_HPP

#include <cerberus/enum.hpp>
#include <cerberus/lexical/sequence_parser.hpp>

namespace cerb::lex {

    template<CharacterLiteral CharT>
    struct Sequence
    {
        using sequence_parser = SequenceParser<CharT>;
        using constant_bitmap = typename sequence_parser::constant_bitmap;
        using str_view = BasicStringView<CharT>;
        using text_iterator = GetIteratorType<TextGenerator<CharT>>;

        CERBERUS_ENUM(Flags, u32, NONE = 0b0, REVERSE = 0b1, PREFIX_OR_POSTFIX = 0b10);
        CERBERUS_ENUM(Rule, u32, NONE = 0, STAR = 1, PLUS = 2, QUESTION = 3);

        constexpr auto setRule(Rule rule) -> void
        {
            if (sequence_rule != Rule::NONE) {
                throw DotItemNotASequenceError("Attempt to set rule for sequence twice!");
            }
            sequence_rule = rule;
        }

        CERBLIB_DECL auto getIterator() const -> text_iterator
        {
            return parser_of_sequence.getIterator();
        }

        constexpr Sequence() = default;
        constexpr Sequence(Flags flags, text_iterator begin, text_iterator end)
          : parser_of_sequence{ available_chars, begin, end }
        {
            if (flags.isSet(Flags::PREFIX_OR_POSTFIX)) {
                is_prefix_or_postfix = true;
            }
            if (flags.isSet(Flags::REVERSE)) {
                parser_of_sequence.reverse();
            }
        }

    private:
        constant_bitmap available_chars{};
        sequence_parser parser_of_sequence{ available_chars, text_iterator(), text_iterator() };
        Rule sequence_rule{ Rule::NONE };
        bool is_prefix_or_postfix{ false };
    };
}// namespace cerb::lex

#endif /* CERBERUS_SEQUENCE_HPP */
