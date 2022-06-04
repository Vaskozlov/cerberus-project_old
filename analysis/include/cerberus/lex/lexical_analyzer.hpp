#ifndef CERBERUS_LEXICAL_ANALYZER_HPP
#define CERBERUS_LEXICAL_ANALYZER_HPP

#include <cerberus/lex/item/item.hpp>
#include <cerberus/string_hash.hpp>
#include <cerberus/string_view.hpp>
#include <map>

namespace cerb::lex
{
    template<CharacterLiteral CharT, CharacterLiteral CharForId = char>
    struct LexicalAnalyzer
    {
        using item_init_pack_t = Pair<BasicStringView<CharForId>, BasicStringView<CharT>>;

        LexicalAnalyzer() = default;

        constexpr LexicalAnalyzer(std::initializer_list<item_init_pack_t> const &items)
        {
            for (item_init_pack_t const &init_pack : items) {
                constructItem(init_pack);
            }

            analysis_globals.lazy_executor.join();
        }

    private:
        auto constructItem(item_init_pack_t const &init_pack) -> void
        {
            analysis_globals.lazy_executor.addJob([&init_pack, this]() {
                this->constructAndAddDotItem(init_pack);
                return std::any(0);
            });
        }

        auto constructAndAddDotItem(item_init_pack_t const &init_pack) -> void
        {
            auto id = hash::hashString(init_pack.first);
            auto const &rule = init_pack.second;

            auto new_item = DotItem<CharT>(analysis_globals, id, rule);
            this->emplaceNewDotItem(id, new_item);
        }

        auto emplaceNewDotItem(size_t id, DotItem<CharT> &item) -> void
        {
            std::scoped_lock lock{ dot_item_mutex };
            dot_items.emplace(id, std::move(item));
        }

        std::map<size_t, DotItem<CharT>> dot_items{};
        AnalysisGlobals<CharT> analysis_globals{};
        std::mutex dot_item_mutex{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_LEXICAL_ANALYZER_HPP */
