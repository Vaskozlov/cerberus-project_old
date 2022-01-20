#ifndef CERBERUS_PARAM_PACK_HPP
#define CERBERUS_PARAM_PACK_HPP

#include <cerberus/lexical/text_generator.hpp>
#include <cerberus/lexical/tokens/comments_token.hpp>
#include <cerberus/lexical/tokens/simple_token.hpp>
#include <cerberus/string_pool.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT, typename TokenType>
    struct ParametersPack
    {
        using str_view_t = BasicStringView<CharT>;
        using string_pool_t = StringPool<CharT, TokenType>;

        using simple_token_t = SimpleToken<CharT, TokenType>;
        using comments_token_t = CommentsTokens<CharT>;

        using generator_t = TextGenerator<CharT>;

        CERBLIB_DECL auto getSingleLineCommentBegin() const -> str_view_t const &
        {
            return comments.single_line_begin;
        }

        CERBLIB_DECL auto getMultilineCommentBegin() const -> str_view_t const &
        {
            return comments.multiline_begin;
        }

        CERBLIB_DECL auto getMultilineCommentEnd() const -> str_view_t const &
        {
            return comments.multiline_end;
        }

        CERBLIB_DECL auto findLongestStringInPool(str_view_t const &string) const -> str_view_t
        {
            return terminals.findLongestStringInPool(string);
        }

        CERBLIB_DECL auto getCurrentStateString() const -> str_view_t
        {
            return generator_for_text.getCurrentStateString();
        }

        constexpr ParametersPack() = default;
        constexpr ParametersPack(
            string_pool_t const &list_of_terminals, comments_token_t const &tokens_for_comments,
            generator_t const &txt_generator, simple_token_t const &token_for_char,
            simple_token_t const &token_for_string)
          : terminals(list_of_terminals), comments(tokens_for_comments),
            generator_for_text(txt_generator), char_token(token_for_char),
            string_token(token_for_string)
        {}

        string_pool_t terminals{};
        comments_token_t comments{};
        generator_t generator_for_text{};
        simple_token_t char_token{};
        simple_token_t string_token{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_PARAM_PACK_HPP */
