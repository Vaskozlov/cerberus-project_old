#ifndef CERBERUS_SEQUENCE_HPP
#define CERBERUS_SEQUENCE_HPP

#include <cerberus/lexical/dot_item_object.hpp>
#include <cerberus/lexical/sequence_parser.hpp>

namespace cerb::lex {
    template<CharacterLiteral CharT, typename TokenType>
    struct Sequence final : public DotItemObject<CharT, TokenType>
    {
        using parent = DotItemObject<CharT, TokenType>;
        using parent::is_prefix_or_postfix;
        using parent::sequence_rule;
        using str = typename parent::str;
        using str_view = typename parent::str_view;
        using Rule = typename parent::Rule;
        using Flags = typename parent::Flags;
        using ScanStatus = typename parent::ScanStatus;
        using text_iterator = typename parent::text_iterator;
        using text_generator = typename parent::text_generator;
        using sequence_parser = SequenceParser<CharT>;
        using constant_bitmap = typename sequence_parser::constant_bitmap;

        constexpr auto scan() -> ScanStatus override
        {
            return ScanStatus{};
        }

        constexpr Sequence(Flags flags, text_iterator &begin, text_iterator const &end)
        {
            sequence_parser parser_of_sequence{ available_chars, begin, end };

            if (flags.isSet(Flags::PREFIX_OR_POSTFIX)) {
                is_prefix_or_postfix = true;
            }
            if (flags.isSet(Flags::REVERSE)) {
                parser_of_sequence.reverse();
            }
        }

    private:
        constant_bitmap available_chars{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_SEQUENCE_HPP */
