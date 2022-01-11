#ifndef CERBERUS_STRING_SEQUENCE_HPP
#define CERBERUS_STRING_SEQUENCE_HPP

#include <cerberus/lexical/dot_item_object.hpp>
#include <string>

namespace cerb::lex {
    template<CharacterLiteral CharT>
    struct StringSequence final : public DotItemObject<CharT>
    {
        using parent = DotItemObject<CharT>;
        using parent::is_prefix_or_postfix;
        using parent::sequence_rule;
        using str = typename parent::str;
        using str_iterator = typename parent::str_view;
        using Rule = typename parent::Rule;
        using Flags = typename parent::Flags;
        using text_iterator = typename parent::text_iterator;
        using text_generator = typename parent::text_generator;
        using string_parser = StringParser<CharT, text_iterator>;

        constexpr auto scan() -> void override
        {}

        constexpr StringSequence(Flags flags, text_iterator &begin, text_iterator const &end)
        {
            string_parser parser_for_string{ static_cast<CharT>('\"'), begin, end };
            parsed_string = std::move(parser_for_string.parseString());

            if (flags.isSet(Flags::PREFIX_OR_POSTFIX)) {
                is_prefix_or_postfix = true;
            }
            if (flags.isSet(Flags::REVERSE)) {
                std::ranges::reverse(parsed_string);
            }
        }

    private:
        str parsed_string{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_STRING_SEQUENCE_HPP */
