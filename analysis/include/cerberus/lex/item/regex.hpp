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

        constexpr explicit RegexItem(
            CERBLIB_BASIC_ITEM_ARGS, BasicStringView<CharT> const &regex_rule)
          : CERBLIB_CONSTRUCT_BASIC_ITEM
        {}
    };
}// namespace cerb::lex::regex

#endif /* CERBERUS_REGEX_HPP */
