#ifndef CERBERUS_DOT_ITEM_OBJECT_HPP
#define CERBERUS_DOT_ITEM_OBJECT_HPP

#include <cerberus/enum.hpp>
#include <cerberus/lexical/generator_for_text.hpp>
#include <cerberus/lexical/lexical_exceptions.hpp>
#include <cerberus/string_view.hpp>
#include <cerberus/type.hpp>

namespace cerb::lex {
    template<CharacterLiteral CharT>
    struct DotItemObject
    {
        using str = std::basic_string<CharT>;
        using str_view = BasicStringView<CharT>;
        using str_iterator = GetIteratorType<str>;
        using str_view_iterator = GetIteratorType<str_view>;
        using text_generator = GeneratorForText<CharT>;
        using text_iterator = GetIteratorType<text_generator>;

        CERBERUS_ENUM(Flags, u32, NONE = 0b0, REVERSE = 0b1, PREFIX_OR_POSTFIX = 0b10);// NOLINT
        CERBERUS_ENUM(Rule, u32, NONE = 0, STAR = 1, PLUS = 2, QUESTION = 3);          // NOLINT

        constexpr virtual auto scan() -> void = 0;

        constexpr auto setGenerator(text_generator const &new_generator) -> void
        {
            generator_for_text = new_generator;
        }

        constexpr auto setRule(Rule rule) -> void
        {
            if (sequence_rule != Rule::NONE) {
                throw DotItemNotASequenceError("Attempt to set rule for sequence twice!");
            }
            sequence_rule = rule;
        }

        constexpr DotItemObject() = default;
        constexpr virtual ~DotItemObject() = default;

    protected:
        text_generator generator_for_text{};
        Rule sequence_rule{ Rule::NONE };
        bool is_prefix_or_postfix{ false };
    };
}// namespace cerb::lex

#endif /* CERBERUS_DOT_ITEM_OBJECT_HPP */
