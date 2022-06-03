#ifndef CERBERUS_ITEM_PARSER_ERRORS_HPP
#define CERBERUS_ITEM_PARSER_ERRORS_HPP

#include <cerberus/lex/item/basic_item.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct ItemParser;

    CERBERUS_EXCEPTION(BasicDotItemParsingError, BasicLexicalAnalysisException);

    template<CharacterLiteral CharT>
    CERBERUS_ANALYSIS_EXCEPTION(DotItemParsingError, CharT, BasicDotItemParsingError);

    template<CharacterLiteral CharT>
    struct ItemParsingErrors
    {
        using item_parser_t = ItemParser<CharT>;

        constexpr static auto mistakeInRegex(item_parser_t const &item_parser) -> void
        {
            throwException("Error in regex during the rule parsing!", item_parser);
        }

        constexpr static auto checkItemNotEmpty(item_parser_t const &item_parser) -> void
        {
            auto &items = item_parser.items;
            auto flags = item_parser.flags;

            if (items.empty() && not flags.isSet(ItemFlags::NONTERMINAL)) {
                throwException("Empty items are not allowed!", item_parser);
            }
        }

        constexpr static auto checkItemIsNotNonterminal(item_parser_t const &item_parser) -> void
        {
            auto &flags = item_parser.flags;

            if (flags.isSet(ItemFlags::NONTERMINAL)) {
                throwException(
                    "Nonterminals can't coexist with other rules and can't be used in recursion!",
                    item_parser);
            }
        }

        constexpr static auto checkThatNonTerminalCanBeAdded(item_parser_t const &item_parser)
            -> void
        {
            auto &items = item_parser.items;

            if (not items.empty()) {
                throwException("Non terminals can't coexist with other rules!", item_parser);
            }
        }

        constexpr static auto checkItemExistence(item_parser_t const &item_parser) -> void
        {
            auto &items = item_parser.items;

            if (items.empty()) {
                throwException(
                    "Unable to apply operation, because current item hasn't"
                    " been created!",
                    item_parser);
            }
        }

        constexpr static auto checkRuleOverloading(item_parser_t const &item_parser) -> void
        {
            constexpr ItemFlags repetition_rules =
                ItemFlags::PLUS | ItemFlags::STAR | ItemFlags::QUESTION;

            auto &items = item_parser.items;
            auto &last_item = items.back();
            auto last_item_flags = last_item->flags;

            if (last_item_flags.isAnyOfSet(repetition_rules)) {
                throwException("Unable to apply more than one rule!", item_parser);
            }
        }

    private:
        constexpr static auto
            throwException(string_view const &message, item_parser_t const &item_parser) -> void
        {
            throw DotItemParsingError(message, item_parser.getGenerator());
        }
    };
}// namespace cerb::lex

#endif /* CERBERUS_ITEM_PARSER_ERRORS_HPP */
