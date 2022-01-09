#ifndef CERBERUS_STRING_SEQUENCE_HPP
#define CERBERUS_STRING_SEQUENCE_HPP

#include <cerberus/lexical/dot_item_object.hpp>
#include <string>

namespace cerb::lex {
    template<CharacterLiteral CharT>
    struct StringSequence final : public DotItemObject<CharT>
    {
        using parent = DotItemObject<CharT>;
        using str = std::basic_string<CharT>;
        using str_iterator = GetIteratorType<str>;
        using text_iterator = typename parent::text_iterator;
        using string_parser = StringParser<CharT, text_iterator>;
        using Rule = typename parent::Rule;
        using Flags = typename parent::Flags;

        constexpr auto setRule(Rule rule) -> void override
        {
            if (sequence_rule != Rule::NONE) {
                throw DotItemNotASequenceError("Attempt to set rule for sequence twice!");
            }
            sequence_rule = rule;
        }

        CERBLIB_DECL auto getIterator() const -> text_iterator override
        {
            return parser_for_string.getIterator();
        }

        constexpr StringSequence(Flags flags, text_iterator begin, text_iterator end)
          : parser_for_string{ static_cast<CharT>('\"'), begin, end }
        {
            parser_for_string.parseString();

            if (flags.isSet(Flags::PREFIX_OR_POSTFIX)) {
                is_prefix_or_postfix = true;
            }
            if (flags.isSet(Flags::REVERSE)) {
                std::ranges::reverse(parsed_string);
            }
        }

    private:
        string_parser parser_for_string{};
        str &parsed_string{ parser_for_string.get() };
        Rule sequence_rule{ Rule::NONE };
        bool is_prefix_or_postfix{ false };
    };
}// namespace cerb::lex

#endif /* CERBERUS_STRING_SEQUENCE_HPP */
