#ifndef CERBERUS_LEXICAL_ANALYZER_HPP
#define CERBERUS_LEXICAL_ANALYZER_HPP

#include <cerberus/lex/item/item.hpp>
#include <cerberus/string_view.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct LexicalAnalyzer
    {
        LexicalAnalyzer() = default;

        constexpr LexicalAnalyzer(
            std::initializer_list<Pair<size_t, BasicStringView<CharT>>> const & /*items*/)
        {}
    };

    extern template struct LexicalAnalyzer<char>;
    extern template struct LexicalAnalyzer<char16_t>;
}// namespace cerb::lex

#endif /* CERBERUS_LEXICAL_ANALYZER_HPP */
