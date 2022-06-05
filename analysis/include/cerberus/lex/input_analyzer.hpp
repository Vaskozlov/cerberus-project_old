#ifndef CERBERUS_INPUT_ANALYZER_HPP
#define CERBERUS_INPUT_ANALYZER_HPP

#include <cerberus/lex/item/item.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT, CharacterLiteral CharForId = char>
    class InputAnalyzer
    {
    public:
        InputAnalyzer() = default;

        constexpr InputAnalyzer(
            text::GeneratorForText<CharT> gen,
            std::map<size_t, BasicStringView<CharForId>> const &id_to_name,
            std::map<size_t, std::vector<DotItem<CharT>>> const &items,
            AnalysisGlobals<CharT> const &globals)
          : generator(std::move(gen)), hash_to_names(id_to_name), dot_items(items),
            analysis_globals(globals)
        {}

    private:
        text::GeneratorForText<CharT> generator{};
        std::map<size_t, BasicStringView<CharForId>> const &hash_to_names;
        std::map<size_t, std::vector<DotItem<CharT>>> const &dot_items;
        AnalysisGlobals<CharT> const &analysis_globals;
    };
}// namespace cerb::lex

#endif /* CERBERUS_INPUT_ANALYZER_HPP */
