#ifndef CERBERUS_BRACKET_FINDER_HPP
#define CERBERUS_BRACKET_FINDER_HPP

#include <cerberus/analysis/analysis_exception.hpp>
#include <cerberus/text_scan_assistance/generator_for_text.hpp>

namespace cerb::text
{
    CERBERUS_EXCEPTION(BasicBracketFinderError, BasicTextScanningException);

    template<CharacterLiteral CharT>
    CERBERUS_ANALYSIS_EXCEPTION(BracketFinderError, CharT, BasicBracketFinderError);

    template<CharacterLiteral CharT>
    class BracketFinder
    {
        template<std::integral Int>
        CERBLIB_DECL static auto cast(Int value) -> CharT
        {
            return value;
        }

    public:
        CERBLIB_DECL auto getPosition() const -> ssize_t
        {
            return getChar() == close_bracket ? static_cast<ssize_t>(generator.getOffset()) : -1;
        }

        BracketFinder() = default;

        constexpr BracketFinder(
            CharT opening_bracket, CharT closing_bracket, GeneratorForText<CharT> const &gen)
          : generator(gen), open_bracket(opening_bracket), close_bracket(closing_bracket)
        {}

        CERBLIB_DECL auto findBracketPosition() -> size_t
        {
            initialize();
            isBeginBracket();
            passed_brackets = 1;

            while (canContinueSearching()) {
                CharT chr = nextChar();
                processChar(chr);
            }

            return generator.getOffset();
        }

    private:
        constexpr auto initialize() -> void
        {
            if (not generator.isInitialized()) {
                generator.getRawChar();
            }
        }

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
            return generator.getCurrentChar();
        }

        constexpr auto nextChar() -> CharT
        {
            return generator.getRawChar();
        }

        CERBLIB_DECL auto canContinueSearching() const -> bool
        {
            if (isCurrentCharEoF()) {
                throw BracketFinderError("Unexpected EoF!", generator);
            }

            return passed_brackets != 0;
        }

        CERBLIB_DECL auto isCurrentCharEoF() const -> bool
        {
            return lex::isEoF(getChar());
        }

        constexpr auto isBeginBracket() const -> void
        {
            if (getChar() != open_bracket) {
                throw BracketFinderError("Unable to find starting bracket!", generator);
            }
        }

        GeneratorForText<CharT> generator{};
        size_t passed_brackets{};
        CharT open_bracket{};
        CharT close_bracket{};
    };

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto findBracket(
        CharT opening_bracket, CharT closing_bracket, GeneratorForText<CharT> const &gen) -> size_t
    {
        BracketFinder<CharT> finder{ opening_bracket, closing_bracket, gen };
        return finder.findBracketPosition();
    }

#ifndef CERBERUS_HEADER_ONLY
    extern template class BracketFinder<char>;
    extern template class BracketFinder<char8_t>;
    extern template class BracketFinder<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::text

#endif /* CERBERUS_BRACKET_FINDER_HPP */
