#ifndef CERBERUS_REGEX_HPP
#define CERBERUS_REGEX_HPP

#include <cerberus/lex/item/basic_item.hpp>
#include <cerberus/lex/item/regex_parser.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct RegexItem : public BasicItem<CharT>
    {
        constexpr explicit RegexItem(BasicStringView<CharT> const &regex_rule)
        {}
    };
}// namespace cerb::lex

#endif /* CERBERUS_REGEX_HPP */
