#ifndef CERBERUS_LEXICAL_ANALYZER_HPP
#define CERBERUS_LEXICAL_ANALYZER_HPP

#include <cerberus/lexical/dot_item.hpp>
#include <cerberus/string_pool.hpp>

namespace cerb::lex {
    template<CharacterLiteral CharT, typename TokenType>
    class LexicalAnalyzer
    {
        using str_view = BasicStringView<CharT>;
        using string_pool = StringPool<CharT, TokenType>;

        struct SimpleToken
        {
            CharT separator{};
            TokenType type{};
        };

        struct CommentsTokens
        {
            str_view single_line_begin{};
            str_view multiline_begin{};
            str_view multiline_end{};
        };

        template<std::integral T>
        CERBLIB_DECL static auto cast(T value) -> CharT
        {
            return static_cast<CharT>(value);
        }

    public:
        constexpr LexicalAnalyzer(
            SimpleToken const &token_for_char, SimpleToken const &token_for_string,
            CommentsTokens const &tokens_of_comments,
            StringPool<CharT, TokenType> const &terminals_pool)
          : terminals(terminals_pool), comments_tokens(tokens_of_comments),
            char_token(token_for_char), string_token(token_for_string)
        {
            std::ranges::for_each(
                terminals_pool, [](const auto &elem) { DotItem<CharT, TokenType>(elem.first); });
        }

    private:
        string_pool terminals{};
        CommentsTokens comments_tokens{};
        SimpleToken char_token{};
        SimpleToken string_token{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_LEXICAL_ANALYZER_HPP */
