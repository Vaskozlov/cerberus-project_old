#ifndef CERBERUS_SEQUENCE_HPP
#define CERBERUS_SEQUENCE_HPP

#include <cerberus/lexical/sequence_parser.hpp>

namespace cerb::lex {

    template<CharacterLiteral CharT>
    struct Sequence
    {
        using sequence_parser = SequenceParser<CharT>;
        using str_view = BasicStringView<CharT>;
        using text_iterator = GetIteratorType<TextGenerator<CharT>>;

        enum struct Flags : u32
        {
            NONE = 0b0,
            REVERSE = 0b1,
            PREFIX_OR_POSTFIX = 0b10
        };

        enum struct Rule
        {
            NONE,
            STAR,
            PLUS,
            QUESTION,
        };

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

        constexpr Sequence(Flags flags, text_iterator begin, text_iterator end)
          : parser_of_sequence{ begin, end }
        {
            if (isFlagSet(flags, Flags::PREFIX_OR_POSTFIX)) {
                is_prefix_or_postfix = true;
            }
            if (isFlagSet(flags, Flags::REVERSE)) {
                parser_of_sequence.reverse();
            }
        }

    private:
        sequence_parser parser_of_sequence{};
        Rule sequence_rule{ Rule::NONE };
        bool is_prefix_or_postfix{ false };
    };
}// namespace cerb::lex

#endif /* CERBERUS_SEQUENCE_HPP */
