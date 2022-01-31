#ifndef CERBERUS_SIMPLE_TOKEN_HPP
#define CERBERUS_SIMPLE_TOKEN_HPP

namespace cerb::lex
{
    template<CharacterLiteral CharT, typename TokenType>
    struct SimpleToken
    {
        TokenType type{};
        CharT separator{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_SIMPLE_TOKEN_HPP */
