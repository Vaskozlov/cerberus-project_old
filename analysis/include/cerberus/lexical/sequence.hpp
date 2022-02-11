#ifndef CERBERUS_SEQUENCE_HPP
#define CERBERUS_SEQUENCE_HPP

#include <cerberus/lexical/dot_item/dot_item_object.hpp>
#include <cerberus/lexical/sequence_parser.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT, typename TokenType>
    struct Sequence final : public DotItemObject<CharT, TokenType>
    {
        using sequence_parser_t = SequenceParser<CharT>;
        using parent_t = DotItemObject<CharT, TokenType>;

        using generator_t = typename parent_t::generator_t;
        using dot_item_pack_t = typename parent_t::dot_item_pack_t;
        using parameters_pack_t = typename parent_t::parameters_pack_t;
        using constant_bitmap_t = typename sequence_parser_t::constant_bitmap_t;

        using parent_t::is_prefix_or_postfix;

        using Flags = typename parent_t::Flags;
        using ScanStatus = typename parent_t::ScanStatus;

        constexpr auto scan() -> ScanStatus override
        {
            return ScanStatus{};
        }

        constexpr Sequence(
            parameters_pack_t const &parameters_for_analysis, Flags flags,
            dot_item_pack_t &dot_item_parameters)
          : parent_t(dot_item_parameters, parameters_for_analysis)
        {
            generator_t &text_generator = dot_item_parameters.text_generator;
            sequence_parser_t parser_of_sequence{ available_chars, ref(text_generator) };

            if (flags.isSet(Flags::PREFIX_OR_POSTFIX)) {
                is_prefix_or_postfix = true;
            }

            if (flags.isSet(Flags::REVERSE)) {
                parser_of_sequence.reverse();
            }
        }

    private:
        constant_bitmap_t available_chars{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_SEQUENCE_HPP */
