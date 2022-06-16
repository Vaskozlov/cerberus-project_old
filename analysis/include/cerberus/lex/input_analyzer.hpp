#ifndef CERBERUS_INPUT_ANALYZER_HPP
#define CERBERUS_INPUT_ANALYZER_HPP

#include <cerberus/lex/item/item.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT, CharacterLiteral CharForId = char>
    struct Rule
    {
        std::vector<DotItem<CharT>> items{};
        BasicStringView<CharForId> name{};
    };

    template<CharacterLiteral CharT, CharacterLiteral CharForId = char>
    class InputAnalyzer
    {
        using rule_t = Rule<CharT, CharForId>;
        using generator_t = text::GeneratorForText<CharT>;

    public:
        InputAnalyzer() = default;

        constexpr InputAnalyzer(
            generator_t gen,
            std::map<size_t, rule_t> const &items,
            AnalysisGlobals<CharT> const &globals)
          : generator(std::move(gen)), dot_items(items), analysis_globals(globals)
        {}

    private:
        generator_t generator{};
        std::map<size_t, rule_t> const &dot_items;
        AnalysisGlobals<CharT> const &analysis_globals;
    };
}// namespace cerb::lex

#endif /* CERBERUS_INPUT_ANALYZER_HPP */
