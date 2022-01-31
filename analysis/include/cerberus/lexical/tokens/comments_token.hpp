#ifndef CERBERUS_COMMENTS_TOKEN_HPP
#define CERBERUS_COMMENTS_TOKEN_HPP

#include <cerberus/string_view.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct CommentsTokens
    {
        using str_view_t = BasicStringView<CharT>;

        str_view_t single_line_begin{};
        str_view_t multiline_begin{};
        str_view_t multiline_end{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_COMMENTS_TOKEN_HPP */
