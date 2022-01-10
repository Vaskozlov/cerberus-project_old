#ifndef CERBERUS_TOKEN_HPP
#define CERBERUS_TOKEN_HPP

#include <cerberus/lexical/generator_for_text.hpp>
#include <string>
#include <vector>

namespace cerb::lex {

    template<CharacterLiteral CharT>
    struct Token
    {
        using TokenType = usize;

        CERBLIB_DECL auto getType() const -> TokenType
        {
            return type;
        }

        CERBLIB_DECL auto getRepr() const -> BasicStringView<CharT> const &
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

        CERBLIB_DECL auto getTabsAndSpaces() const -> std::basic_string<CharT> const &
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

        virtual auto transformToken() -> void
        {}

        constexpr Token() = default;
        constexpr virtual ~Token() = default;
        constexpr Token(
            TokenType type_of_token, LocationInFile const &location_in_file,
            BasicStringView<CharT> const &repr_of_token, GeneratorForText<CharT> const &txt_manager)
          : location(location_in_file), repr(repr_of_token), line(txt_manager.getCurrentLine()),
            tabs_and_spaces(txt_manager.getTabsAndSpaces()), type(type_of_token)
        {}

    private:
        LocationInFile location{};
        BasicStringView<CharT> repr{};
        BasicStringView<CharT> line{};
        std::basic_string<CharT> tabs_and_spaces{};
        TokenType type{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_TOKEN_HPP */
