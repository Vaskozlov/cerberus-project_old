#ifndef CERBERUS_REGEX_HPP
#define CERBERUS_REGEX_HPP

#include <cerberus/lex/item/basic_item.hpp>
#include <cerberus/lex/item/regex_parser.hpp>

namespace cerb::lex::regex
{
    template<CharacterLiteral CharT>
    struct RegexItem : public BasicItem<CharT>
    {
        CERBLIB_BASIC_ITEM_ACCESS(CharT);
        using bitmap_t = typename RegexParser<CharT>::bitmap_t;

        constexpr explicit RegexItem(CERBLIB_BASIC_ITEM_ARGS, GeneratorForText<CharT> &regex_rule)
          : CERBLIB_CONSTRUCT_BASIC_ITEM
        {
            parseRegex(regex_rule);
        }

    private:
        constexpr auto parseRegex(GeneratorForText<CharT> &regex_rule) -> void
        {
            RegexParser<CharT> regex_parser{ regex_rule, available_chars };
        }

        bitmap_t available_chars{};
    };
}// namespace cerb::lex::regex

#endif /* CERBERUS_REGEX_HPP */
