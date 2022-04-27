#ifndef CERBERUS_BRACKET_FINDER_HPP
#define CERBERUS_BRACKET_FINDER_HPP

#include <cerberus/lex/generator_for_text.hpp>

namespace cerb::lex
{
    CERBERUS_EXCEPTION(BracketFinderError);

    template<CharacterLiteral CharT>
    class BracketFinder
    {
        template<std::integral T>
        CERBLIB_DECL static auto cast(T value) -> CharT
        {
            return value;
        }

    public:
        CERBLIB_DECL auto getPosition() const -> ssize_t
        {
            return getChar() == close_bracket ? static_cast<ssize_t>(index) : -1;
        }

        BracketFinder() = default;

        constexpr BracketFinder(
            CharT opening_bracket, CharT closing_bracket, BasicStringView<CharT> const &str)
          : text(str), open_bracket(opening_bracket), close_bracket(closing_bracket)
        {}

        constexpr auto findBracketPosition() -> size_t
        {
            isBeginBracket();
            passed_brackets = 1;

            while (canContinueSearching()) {
                CharT chr = getChar();
                processChar(chr);
            }

            return index - 1;
        }

    private:
        constexpr auto processChar(CharT chr) -> void
        {
            if (chr == open_bracket) {
                ++passed_brackets;
            } else if (chr == close_bracket) {
                --passed_brackets;
            }
        }

        CERBLIB_DECL auto getChar() const -> CharT
        {
            return text[index];
        }

        constexpr auto nextChar() -> CharT
        {
            return text[++index];
        }

        CERBLIB_DECL auto canContinueSearching() -> bool
        {
            if (isCurrentCharEoF()) {
                throw BracketFinderError("Unexpected EoF");
            }

            nextChar();
            return passed_brackets != 0;
        }

        CERBLIB_DECL auto isCurrentCharEoF() const -> bool
        {
            return isEoF(getChar());
        }

        constexpr auto isBeginBracket() const -> void
        {
            if (getChar() != open_bracket) {
                throw BracketFinderError("Unable to find starting bracket!");
            }
        }

        BasicStringView<CharT> text{};
        size_t passed_brackets{};
        size_t index{};
        CharT open_bracket{};
        CharT close_bracket{};
    };

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto
        findBracket(CharT opening_bracket, CharT closing_bracket, BasicStringView<CharT> const &str)
            -> size_t
    {
        BracketFinder<CharT> finder{ opening_bracket, closing_bracket, str };
        return finder.findBracketPosition();
    }

#ifndef CERBERUS_HEADER_ONLY
    extern template class BracketFinder<char>;
    extern template class BracketFinder<char8_t>;
    extern template class BracketFinder<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::lex

#endif /* CERBERUS_BRACKET_FINDER_HPP */
