#ifndef CERBERUS_SEQUENCE_HPP
#define CERBERUS_SEQUENCE_HPP

#include <cerberus/lexical/dot_item_object.hpp>
#include <cerberus/lexical/sequence_parser.hpp>

namespace cerb::lex {
    template<CharacterLiteral CharT>
    struct Sequence final : public DotItemObject<CharT>
    {
        using parent = DotItemObject<CharT>;
        using sequence_parser = SequenceParser<CharT>;
        using constant_bitmap = typename sequence_parser::constant_bitmap;
        using str_view = typename parent::str_view;
        using text_iterator = typename parent::text_iterator;
        using Rule = typename parent::Rule;
        using Flags = typename parent::Flags;

        constexpr auto setRule(Rule rule) -> void override
        {
            if (sequence_rule != Rule::NONE) {
                throw DotItemNotASequenceError("Attempt to set rule for sequence twice!");
            }
            sequence_rule = rule;
        }

        constexpr Sequence(Flags flags, text_iterator &begin, text_iterator const &end)
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
