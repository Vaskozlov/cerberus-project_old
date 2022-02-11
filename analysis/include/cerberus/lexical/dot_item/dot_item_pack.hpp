#ifndef CERBERUS_DOT_ITEM_PACK_HPP
#define CERBERUS_DOT_ITEM_PACK_HPP

#include <cerberus/lexical/text_generator.hpp>
#include <cerberus/lexical/tokens/token.hpp>
#include <cerberus/reference_wrapper.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT, LessComparable TokenType>
    struct DotItemParameters
    {
        using token_t = Token<CharT, TokenType>;
        using generator_t = TextGenerator<CharT>;

        template<typename... Ts>
        constexpr explicit DotItemParameters(Ts &&...generator_args)
          : text_generator(std::forward<Ts>(generator_args)...)
        {}

        token_t recognized_token{};
        generator_t text_generator{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_DOT_ITEM_PACK_HPP */
