#ifndef CERBERUS_ITEM_PARSER_ERRORS_HPP
#define CERBERUS_ITEM_PARSER_ERRORS_HPP

#include <cerberus/lex/item/basic_item.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct DotItem;

    CERBERUS_EXCEPTION(BasicDotItemParsingError, BasicLexicalAnalysisException);

    template<CharacterLiteral CharT>
    CERBERUS_ANALYSIS_EXCEPTION(DotItemParsingError, CharT, BasicDotItemParsingError);

    template<CharacterLiteral CharT>
    struct DotItemChecks
    {
        using dot_item_t = DotItem<CharT>;

        constexpr static auto mistakeInRegex(dot_item_t const &dot_item) -> void
        {
            throwException("Check in regex during the rule parsing!", dot_item);
        }

        constexpr static auto itemNotEmpty(dot_item_t const &dot_item) -> void
        {
            auto &items = dot_item.items;
            auto flags = dot_item.flags;

            if (items.empty() && not flags.isSet(ItemFlags::NONTERMINAL)) {
                throwException("Empty items are not allowed!", dot_item);
            }
        }

        constexpr static auto itemIsNotNonterminal(dot_item_t const &dot_item) -> void
        {
            auto &flags = dot_item.flags;

            if (flags.isSet(ItemFlags::NONTERMINAL)) {
                throwException(
                    "Nonterminals can't coexist with other rules and can't be used in recursion!",
                    dot_item);
            }
        }

        constexpr static auto nonTerminalCanBeAdded(dot_item_t const &dot_item) -> void
        {
            auto &items = dot_item.items;

            if (not items.empty()) {
                throwException("Non terminals can't coexist with other rules!", dot_item);
            }
        }

        constexpr static auto itemExistence(dot_item_t const &dot_item) -> void
        {
            auto &items = dot_item.items;

            if (items.empty()) {
                throwException(
                    "Unable to apply operation, because current item hasn't"
                    " been created!",
                    dot_item);
            }
        }

        constexpr static auto ruleOverloading(dot_item_t const &dot_item) -> void
        {
            constexpr ItemFlags repetition_rules =
                ItemFlags::PLUS | ItemFlags::STAR | ItemFlags::QUESTION;

            auto &items = dot_item.items;
            auto &last_item = items.back();
            auto last_item_flags = last_item->flags;

            if (last_item_flags.isAnyOfSet(repetition_rules)) {
                throwException("Unable to apply more than one rule!", dot_item);
            }
        }

    private:
        constexpr static auto throwException(string_view const &message, dot_item_t const &dot_item)
            -> void
        {
            throw DotItemParsingError(message, dot_item.getGenerator());
        }
    };
}// namespace cerb::lex

#endif /* CERBERUS_ITEM_PARSER_ERRORS_HPP */
