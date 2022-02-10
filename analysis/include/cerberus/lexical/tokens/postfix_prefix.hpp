#ifndef CERBERUS_POSTFIX_PREFIX_HPP
#define CERBERUS_POSTFIX_PREFIX_HPP

#include <cerberus/enum.hpp>
#include <cerberus/string_view.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct PostfixPrefix
    {
        using str_view_t = BasicStringView<CharT>;

        // NOLINTNEXTLINE
        CERBERUS_ENUM(PrefixOrPostfixVariant, u8, NONE = 0, PREFIX = 1, POSTFIX = 2);

        str_view_t repr{};
        PrefixOrPostfixVariant variant{ PrefixOrPostfixVariant::NONE };
    };
}// namespace cerb::lex

#endif /* CERBERUS_POSTFIX_PREFIX_HPP */
