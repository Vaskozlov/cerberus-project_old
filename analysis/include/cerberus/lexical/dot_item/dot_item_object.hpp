#ifndef CERBERUS_DOT_ITEM_OBJECT_HPP
#define CERBERUS_DOT_ITEM_OBJECT_HPP

#include <cerberus/enum.hpp>
#include <cerberus/lexical/dot_item/param_pack.hpp>
#include <cerberus/lexical/lexical_exceptions.hpp>
#include <cerberus/reference_wrapper.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT, typename TokenType>
    struct DotItemObject
    {
        using str_view_t = BasicStringView<CharT>;

        using generator_t = TextGenerator<CharT>;
        using string_pool_t = StringPool<CharT, TokenType>;

        using parameters_pack_t = ParametersPack<CharT, TokenType>;
        using generator_reference_t = ReferenceWrapper<generator_t>;

        CERBERUS_ENUM(Flags, u16, NONE = 0b1, REVERSE = 0b10, PREFIX_OR_POSTFIX = 0b100);// NOLINT
        CERBERUS_ENUM(Rule, u16, NONE = 0, STAR = 1, PLUS = 2, QUESTION = 3);            // NOLINT
        CERBERUS_ENUM(ScanStatus, u16, REDUCED = 0b1, ERROR = 0b10);                     // NOLINT

        constexpr virtual auto scan() -> ScanStatus = 0;

        constexpr auto setGenerator(ReferenceWrapper<generator_t> new_generator) -> void
        {
            text_generator = new_generator.get();
        }

        constexpr auto setRule(Rule rule) -> void
        {
            if (sequence_rule != Rule::NONE) {
                throw SequenceError("Attempt to set rule for sequence twice!");
            }
            sequence_rule = rule;
        }

        CERBLIB_DECL auto isNextTokenTerminal() const -> bool
        {
            return not analysis_parameters
                           .findLongestStringInPool(analysis_parameters.getCurrentStateString())
                           .empty();
        }

        constexpr DotItemObject() = default;
        constexpr virtual ~DotItemObject() = default;

        constexpr explicit DotItemObject(
            generator_reference_t generator_for_text,
            parameters_pack_t const &parameters_for_analysis)
          : text_generator(generator_for_text.get()), analysis_parameters(parameters_for_analysis)
        {}

    protected:
        generator_t &text_generator;
        parameters_pack_t const &analysis_parameters;
        Rule sequence_rule{ Rule::NONE };
        bool is_prefix_or_postfix{ false };
    };
}// namespace cerb::lex

#endif /* CERBERUS_DOT_ITEM_OBJECT_HPP */
