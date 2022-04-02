#ifndef CERBERUS_LEXICAL_ANALYZER_HPP
#define CERBERUS_LEXICAL_ANALYZER_HPP

#include <cerberus/lex/item/item.hpp>
#include <cerberus/string_view.hpp>
#include <fmt/format.h>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct LexicalAnalyzer
    {
        LexicalAnalyzer() = default;

        constexpr LexicalAnalyzer(
            std::initializer_list<Pair<size_t, BasicStringView<CharT>>> const &items)
        {
            for (auto const &elem : items) {
                fmt::print("id: {}, rule: {}\n", elem.first, elem.second.strView());
            }
        }
    };
}// namespace cerb::lex

#endif /* CERBERUS_LEXICAL_ANALYZER_HPP */
