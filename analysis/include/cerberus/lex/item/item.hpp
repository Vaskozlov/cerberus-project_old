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
        constexpr auto asStringItem(BasicItem<CharT> *item) -> string::StringItem<CharT> *
        {
            return dynamic_cast<string::StringItem<CharT> *>(item);
        }

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
            checkRuleOverloading();
            recent_item->flags |= new_rule;
        }

        constexpr auto addString() -> void
        {
            createNewItem<string::StringItem<CharT>>(rule_generator);
        }

        constexpr auto addItem() -> void
        {}

        constexpr auto addRegex() -> void
        {
            createNewItem<regex::RegexItem<CharT>>(rule_generator);
        }

        template<typename T, typename... Ts>
        constexpr auto createNewItem(Ts &&...args) -> BasicItem<CharT> *
        {
            static_assert(std::is_base_of_v<BasicItem<CharT>, T>);

            item_ptr new_item = allocateNewItem<T>(std::forward<Ts>(args)...);
            recent_item = new_item.get();

            items.emplace_back(std::move(new_item));

            return recent_item;
        }

        template<typename T, typename... Ts>
        constexpr auto allocateNewItem(Ts &&...args) -> item_ptr
        {
            static_assert(std::is_base_of_v<BasicItem<CharT>, T>);

            return std::make_unique<T>(analysis_globals, std::forward<Ts>(args)...);
        }

        constexpr auto addNonTerminal() -> void
        {
            checkThatNonTerminalCanBeAdded();

            StringToCodes<CharT> string_parser(cast('\''), rule_generator);
            std::basic_string<CharT> &str = string_parser.parseString();

            makeNonterminalGlobal(std::move(str));

            flags |= ItemFlags::NONTERMINAL;
        }

        constexpr auto makeNonterminalGlobal(std::basic_string<CharT> &&str) -> void
        {
            analysis_globals.emplaceNonterminal(std::move(str), id());
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

        constexpr auto checkRuleOverloading() const -> void
        {
            ItemFlags repetition_rules = ItemFlags::PLUS | ItemFlags::STAR | ItemFlags::QUESTION;

            if ((recent_item->flags & repetition_rules) != ItemFlags::NONE) {
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
