#ifndef CERBERUS_DOT_ITEM_OBJECT_HPP
#define CERBERUS_DOT_ITEM_OBJECT_HPP

#include <cerberus/enum.hpp>
#include <cerberus/lexical/generator_for_text.hpp>
#include <cerberus/lexical/lexical_exceptions.hpp>
#include <cerberus/string_pool.hpp>
#include <cerberus/string_view.hpp>
#include <cerberus/type.hpp>

namespace cerb::lex {
    template<CharacterLiteral CharT, typename TokenType>
    struct DotItemObject
    {
        using str = std::basic_string<CharT>;
        using str_view = BasicStringView<CharT>;
        using string_pool = StringPool<CharT, TokenType>;
        using str_iterator = GetIteratorType<str>;
        using str_view_iterator = GetIteratorType<str_view>;
        using text_generator = GeneratorForText<CharT>;
        using text_iterator = GetIteratorType<text_generator>;

        CERBERUS_ENUM(Flags, u32, NONE = 0b1, REVERSE = 0b10, PREFIX_OR_POSTFIX = 0b100);// NOLINT
        CERBERUS_ENUM(Rule, u32, NONE = 0, STAR = 1, PLUS = 2, QUESTION = 3);            // NOLINT
        CERBERUS_ENUM(ScanStatus, u32, SHIFTED = 0b1, REDUCED = 0b10, ERROR = 0b100);    // NOLINT

        struct CommentsTokens
        {
            str_view single_line_begin{};
            str_view multiline_begin{};
            str_view multiline_end{};
        };

        struct ParametersPack
        {
            string_pool terminals{};
            CommentsTokens comments{};
            text_iterator iterator_for_text{};
        };

        constexpr virtual auto scan() -> ScanStatus = 0;

        constexpr auto setIterator(text_iterator *new_iterator) -> void
        {
            iterator_for_text = new_iterator;
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
        text_iterator *iterator_for_text{};
        Rule sequence_rule{ Rule::NONE };
        bool is_prefix_or_postfix{ false };
    };
}// namespace cerb::lex

#endif /* CERBERUS_DOT_ITEM_OBJECT_HPP */
