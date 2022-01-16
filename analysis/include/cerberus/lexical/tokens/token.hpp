#ifndef CERBERUS_TOKEN_HPP
#define CERBERUS_TOKEN_HPP

#include <cerberus/lexical/text_generator.hpp>
#include <cerberus/lexical/tokens/postfix_prefix.hpp>
#include <string>
#include <vector>

namespace cerb::lex {

    template<CharacterLiteral CharT, typename TokenType>
    struct Token
    {
        using str_t = std::basic_string<CharT>;
        using str_view_t = BasicStringView<CharT>;

        using variant_t = typename PostfixPrefix<CharT>::PrefixOrPostfixVariant;

        CERBLIB_DECL auto getType() const -> TokenType
        {
            return type;
        }

        CERBLIB_DECL auto getRepr() const -> str_view_t const &
        {
            return repr;
        }

        CERBLIB_DECL auto getLine() const -> size_t
        {
            return location.getLine();
        }

        CERBLIB_DECL auto getCharacter() const -> size_t
        {
            return location.getCharacterInLine();
        }

        CERBLIB_DECL auto getOffset() const -> size_t
        {
            return location.getOffset();
        }

        CERBLIB_DECL auto getFilename() const -> string_view const &
        {
            return location.getFilename();
        }

        CERBLIB_DECL auto getLocation() const -> LocationInFile const &
        {
            return location;
        }

        CERBLIB_DECL auto getTabsAndSpaces() const -> str_t const &
        {
            return tabs_and_spaces;
        }

        CERBLIB_DECL auto operator==(Token const &other) const -> bool
        {
            return type == other.type;
        }

        CERBLIB_DECL auto operator<=>(Token const &other) const
        {
            return type <=> other.type;
        }

        CERBLIB_DECL auto operator==(TokenType const &other) const -> bool
        {
            return type == other;
        }

        CERBLIB_DECL auto operator<=>(TokenType const &other) const
        {
            return type <=> other;
        }

        constexpr auto addPrefix(str_view_t const &prefix_repr) -> void
        {
            prefixes_and_postfixes.emplace_back(prefix_repr, variant_t::PREFIX);
        }

        constexpr auto addPostfix(str_view_t const &prefix_repr) -> void
        {
            prefixes_and_postfixes.emplace_back(prefix_repr, variant_t::POSTFIX);
        }

        constexpr Token() = default;
        constexpr Token(
            TokenType type_of_token, LocationInFile const &location_in_file,
            str_view_t const &repr_of_token, GeneratorForText<CharT> const &txt_manager)
          : location(location_in_file), tabs_and_spaces(txt_manager.getTabsAndSpaces()),
            repr(repr_of_token), line(txt_manager.getCurrentLine()), type(type_of_token)
        {}

    private:
        LocationInFile location{};
        str_t tabs_and_spaces{};
        std::vector<str_view_t> prefixes_and_postfixes{};
        str_view_t repr{};
        str_view_t line{};
        TokenType type{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_TOKEN_HPP */
