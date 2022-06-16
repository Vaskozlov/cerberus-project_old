#ifndef CERBERUS_TOKEN_HPP
#define CERBERUS_TOKEN_HPP

#include <cerberus/text/generator_for_text.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    class Token
    {
    public:
        CERBLIB_DECL auto getId() const -> size_t
        {
            return id;
        }

        Token() = default;

        constexpr explicit Token(text::GeneratorForText<CharT> const &gen) : generator(gen)
        {}

    private:
        text::GeneratorForText<CharT> const generator{};
        size_t id{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_TOKEN_HPP */
