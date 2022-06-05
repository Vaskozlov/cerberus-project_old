#ifndef CERBERUS_LEXICAL_ANALYZER_HPP
#define CERBERUS_LEXICAL_ANALYZER_HPP

#include <cerberus/lex/input_analyzer.hpp>
#include <cerberus/string_hash.hpp>
#include <forward_list>
#include <map>

namespace cerb::lex
{
    template<CharacterLiteral CharT, CharacterLiteral CharForId = char>
    class LexicalAnalyzer
    {
    private:
        struct InitPack
        {
            BasicStringView<CharForId> rule_name{};
            BasicStringView<CharT> rule{};
        };

    public:
        constexpr auto addSource(BasicStringView<CharT> const &input) -> void
        {
            InputAnalyzer<CharT, CharForId> input_analyzer{ text::GeneratorForText{ input },
                                                            hash_to_names, dot_items,
                                                            analysis_globals };
        }

        LexicalAnalyzer() = default;

        constexpr LexicalAnalyzer(std::initializer_list<InitPack> const &items)
        {
            for (InitPack const &init_pack : items) {
                constructItem(init_pack);
            }

            analysis_globals.lazy_executor.join();
        }

    private:
        auto constructItem(InitPack const &init_pack) -> void
        {
            analysis_globals.lazy_executor.addJob([&init_pack, this]() {
                this->constructAndAddDotItem(init_pack);
                return std::any{};
            });
        }

        auto constructAndAddDotItem(InitPack const &init_pack) -> void
        {
            auto const &rule = init_pack.rule;
            size_t id = hash::hashString(init_pack.rule_name);

            DotItem<CharT> new_item = { analysis_globals, id, rule };
            emplaceNewDotItem(id, init_pack.rule_name, new_item);
        }

        auto emplaceNewDotItem(
            size_t id, BasicStringView<CharForId> const &rule_name, DotItem<CharT> &item) -> void
        {
            std::scoped_lock lock{ dot_item_mutex };

            if (not dot_items.contains(id)) {
                dot_items.emplace(id, std::vector<DotItem<CharT>>{});
                hash_to_names[id] = rule_name;
            }

            dot_items[id].push_back(std::move(item));
        }

        std::map<size_t, BasicStringView<CharForId>> hash_to_names{};
        std::map<size_t, std::vector<DotItem<CharT>>> dot_items{};
        AnalysisGlobals<CharT> analysis_globals{};
        std::mutex dot_item_mutex{};
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template class LexicalAnalyzer<char>;
    extern template class LexicalAnalyzer<char8_t>;
    extern template class LexicalAnalyzer<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::lex

#endif /* CERBERUS_LEXICAL_ANALYZER_HPP */
