#ifndef CERBERUS_ITEM_HPP
#define CERBERUS_ITEM_HPP

#include <cerberus/lex/item/regex.hpp>
#include <cerberus/lex/item/string.hpp>
#include <fmt/format.h>
#include <vector>

namespace cerb::lex
{
    CERBERUS_EXCEPTION(DotItemParsingError);

    template<CharacterLiteral CharT>
    struct DotItem : public BasicItem<CharT>
    {
        CERBLIB_BASIC_ITEM_ACCESS(CharT);
        using item_ptr = std::unique_ptr<BasicItem<CharT>>;

        CERBLIB_DECL auto id() const -> size_t
        {
            return item_id;
        }

        constexpr DotItem(
            size_t id, BasicStringView<CharT> const &rule,
            AnalysisGlobals<CharT> &analysis_parameters)
          : parent(analysis_parameters), rule_generator(rule), item_id(id)
        {
            parseRule();
        }

    private:
        constexpr auto parseRule() -> void
        {
            while (not isEoF(rule_generator.getCleanChar())) {
                checkForAlreadyExistsNonterminal();
                processChar(rule_generator.getCurrentChar());
            }
        }

        constexpr auto processChar(CharT chr) -> void
        {
            switch (chr) {
            case cast('\''):
                addNonTerminal();
                break;

            case cast('\"'):
                addString();
                break;

            case cast('('):
                addItem();
                break;

            case cast('['):
                addRegex();
                break;

            case cast('{'):
                setRepetitionRule(ItemFlags::FIXED);
                break;

            case cast('+'):
                setRepetitionRule(ItemFlags::PLUS);
                break;

            case cast('*'):
                setRepetitionRule(ItemFlags::STAR);
                break;

            case cast('?'):
                setRepetitionRule(ItemFlags::QUESTION);
                break;

            case cast('p'):
                setTag(ItemFlags::PREFIX);
                break;

            case cast('r'):
                setTag(ItemFlags::REVERSE);
                break;

            default:
                throw DotItemParsingError("Error in regex during the rule parsing");
            }
        }

        constexpr auto setTag(ItemFlags new_tag) -> void
        {
            checkItemExistence();
            recent_item->flags |= new_tag;
        }

        constexpr auto setRepetitionRule(ItemFlags new_rule) -> void
        {
            checkItemExistence();
            checkRuleOverloading(new_rule);
            recent_item->flags |= new_rule;
        }

        constexpr auto addString() -> void
        {}

        constexpr auto addItem() -> void
        {}

        constexpr auto addRegex() -> void
        {}

        template<typename T, typename... Ts>
        constexpr auto allocateNewItem(Ts &&...args) -> std::unique_ptr<BasicItem<CharT>>
        {
            static_assert(std::is_base_of_v<BasicItem<CharT>, T>);

            return std::make_unique<T>(std::forward(args)...);
        }

        constexpr auto addNonTerminal() -> void
        {
            checkThatNonTerminalCanBeAdded();

            BasicStringView<CharT> const &nonterminal_repr = rule_generator.getRestOfTheText();
            StringToCodes<CharT> string_parser(cast('\''), nonterminal_repr);
            std::basic_string<CharT> &str = string_parser.parseString();

            skipNonterminalFromStream(string_parser);
            makeNonterminalGlobal(std::move(str));

            flags |= ItemFlags::NONTERMINAL;
        }

        constexpr auto makeNonterminalGlobal(std::basic_string<CharT> &&str) -> void
        {
            analysis_globals.emplaceNonterminal(std::move(str), id());
        }

        constexpr auto skipNonterminalFromStream(StringToCodes<CharT> const &string_parser) -> void
        {
            rule_generator.skip(string_parser.getProcessedLength());
        }

        constexpr auto checkForAlreadyExistsNonterminal() const -> void
        {
            if (flags.isSet(ItemFlags::NONTERMINAL)) {
                throw DotItemParsingError("Nonterminals can't coexist with other rules");
            }
        }

        constexpr auto checkThatNonTerminalCanBeAdded() const -> void
        {
            if (recent_item != nullptr) {
                throw DotItemParsingError("Non terminals can't coexist with other rules");
            }
        }

        constexpr auto checkItemExistence() const -> void
        {
            if (recent_item == nullptr) {
                throw DotItemParsingError(
                    "Unable to apply operation, because current item hasn't"
                    " been created");
            }
        }

        constexpr auto checkRuleOverloading(ItemFlags new_rule) const -> void
        {
            ItemFlags other_repetition_rules =
                ItemFlags::PLUS | ItemFlags::STAR | ItemFlags::QUESTION;

            other_repetition_rules.clearFlag(new_rule);

            if ((recent_item->flags & other_repetition_rules) > 0) {
                throw DotItemParsingError("Unable to apply more than one rule!");
            }
        }

        GeneratorForText<CharT> rule_generator{};
        std::vector<item_ptr> items{};
        BasicItem<CharT> *recent_item{};
        size_t item_id{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_ITEM_HPP */
