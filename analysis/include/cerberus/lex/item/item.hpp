#ifndef CERBERUS_ITEM_HPP
#define CERBERUS_ITEM_HPP

#include <cerberus/lex/item/item_parser.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct DotItem : public ItemParser<CharT>
    {
        CERBLIB_ITEM_PARSER_ACCESS(CharT);

        CERBLIB_DECL auto operator==(DotItem const &other) const -> bool
        {
            return getId() == other.getId();
        }

        CERBLIB_DECL auto operator<=>(DotItem const &other) const -> decltype(auto)
        {
            return getId() <=> other.getId();
        }

        constexpr DotItem(
            AnalysisGlobals<CharT> &analysis_parameters, size_t id_of_item,
            BasicStringView<CharT> const &rule)
          : item_parser_t(analysis_parameters, id_of_item, rule)
        {}
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct DotItem<char>;
    extern template struct DotItem<char8_t>;
    extern template struct DotItem<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::lex

#endif /* CERBERUS_ITEM_HPP */
