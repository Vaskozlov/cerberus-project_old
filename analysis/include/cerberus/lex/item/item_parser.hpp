#ifndef CERBERUS_ITEM_PARSER_HPP
#define CERBERUS_ITEM_PARSER_HPP

#include <cerberus/lex/item/alloc.hpp>
#include <cerberus/lex/item/regex.hpp>
#include <cerberus/lex/item/string.hpp>
#include <cerberus/text/bracket_finder.hpp>
#include <utility>

#define CERBLIB_ITEM_PARSER_ACCESS(CharT)                                                          \
    using item_parser_t = cerb::lex::ItemParser<CharT>;                                            \
    using typename item_parser_t::item_ptr;                                                        \
    using item_parser_t::id;                                                                       \
    using item_parser_t::cast;                                                                     \
    using item_parser_t::flags;                                                                    \
    using item_parser_t::item_id;                                                                  \
    using item_parser_t::items;                                                                    \
    using item_parser_t::rule_generator;                                                           \
    using item_parser_t::analysis_globals

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct ItemParser
      : public BasicItem<CharT>
      , private text::ScanApi<true, CharT>
    {
        CERBLIB_BASIC_ITEM_ACCESS(CharT);
        CERBLIB_SCAN_API_ACCESS(true, CharT);
        CERBERUS_ANALYSIS_EXCEPTION(DotItemParsingError, CharT, BasicLexicalAnalysisException);

        using item_ptr = std::unique_ptr<BasicItem<CharT>>;

        CERBLIB_DECL auto id() const -> size_t
        {
            return item_id;
        }

        CERBLIB_DECL auto getItems() const -> std::vector<item_ptr> const &
        {
            return items;
        }

        constexpr ItemParser(
            CERBLIB_BASIC_ITEM_ARGS,
            size_t id_of_item,
            BasicStringView<CharT> const &rule)
          : CERBLIB_CONSTRUCT_BASIC_ITEM, scan_api_t(rule_generator), rule_generator(rule),
            item_id(id_of_item)
        {
            scan_api_t::beginScanning(CharEnum<CharT>::EoF);
        }

        constexpr ItemParser(
            CERBLIB_BASIC_ITEM_ARGS,
            size_t id_of_item,
            text::GeneratorForText<CharT>
                gen)
          : CERBLIB_CONSTRUCT_BASIC_ITEM, scan_api_t(rule_generator),
            rule_generator(std::move(gen)), item_id(id_of_item)
        {
            scan_api_t::beginScanning(CharEnum<CharT>::EoF);
        }

    private:
        constexpr auto onStart() -> text::ScanApiStatus override
        {
            return text::ScanApiStatus::DO_NOT_SKIP_CHAR;
        }

        constexpr auto processChar(CharT chr) -> void override
        {
            checkItemIsNotNonterminal();

            switch (chr) {
            case cast('\''):
                addNonTerminal();
                break;

            case cast('\"'):
                completeLastItem();
                addString();
                break;

            case cast('('):
                completeLastItem();
                addItemParser();
                break;

            case cast('['):
                completeLastItem();
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

            case cast('^'):
                setTag(ItemFlags::REVERSE);
                break;

            default:
                throw DotItemParsingError(
                    "Error in regex during the rule parsing!", getGenerator());
            }
        }

        constexpr auto onEnd() -> void override
        {
            checkItemNotEmpty();
        }

        constexpr auto postInitializationSetup() -> void override
        {
            if (flags.isSet(ItemFlags::REVERSE)) {
                throw DotItemParsingError("Unable to apply reverse rule to item.", getGenerator());
            }
        }

        constexpr auto setTag(ItemFlags new_tag) -> void
        {
            checkItemExistence();

            auto &last_item = items.back();
            last_item->flags |= new_tag;
        }

        constexpr auto setRepetitionRule(ItemFlags new_rule) -> void
        {
            checkItemExistence();
            checkRuleOverloading();

            auto &last_item = items.back();
            last_item->flags |= new_rule;
        }

        constexpr auto addString() -> void
        {
            allocator.newString(rule_generator);
        }

        constexpr auto addItemParser() -> void
        {
            constexpr size_t item_begin_length = 1;

            size_t border = getBorder();
            text::GeneratorForText<CharT> forked_gen =
                rule_generator.fork(item_begin_length, border);
            auto *new_item = allocator.newItemParser(id(), forked_gen);

            new_item->checkItemIsNotNonterminal();
            skipItemBorder(border);
        }

        constexpr auto addRegex() -> void
        {
            allocator.newRegex(rule_generator);
        }

        constexpr auto addNonTerminal() -> void
        {
            checkThatNonTerminalCanBeAdded();

            std::basic_string<CharT> converted_str =
                convertStringToCodes(cast('\''), rule_generator);
            makeNonterminalGlobal(std::move(converted_str));

            flags |= ItemFlags::NONTERMINAL;
        }

        constexpr auto skipItemBorder(size_t border) -> void
        {
            scan_api_t::skip(border);
        }

        CERBLIB_DECL auto getBorder() const -> size_t
        {
            return findBracket(cast('('), cast(')'), getGenerator());
        }

        constexpr auto completeLastItem() -> void
        {
            if (not items.empty()) {
                auto &last_item = items.back();
                last_item->postInitializationSetup();
            }
        }

        constexpr auto makeNonterminalGlobal(std::basic_string<CharT> &&str) -> void
        {
            analysis_globals.emplaceNonterminal(std::move(str), id());
        }

        constexpr auto checkItemNotEmpty() const -> void
        {
            if (items.empty() && not flags.isSet(ItemFlags::NONTERMINAL)) {
                throw DotItemParsingError("Empty items are not allowed!", getGenerator());
            }
        }

        constexpr auto checkItemIsNotNonterminal() const -> void
        {
            if (flags.isSet(ItemFlags::NONTERMINAL)) {
                throw DotItemParsingError(
                    "Nonterminals can't coexist with other rules and can't be used in recursion!",
                    getGenerator());
            }
        }

        constexpr auto checkThatNonTerminalCanBeAdded() const -> void
        {
            if (not items.empty()) {
                throw DotItemParsingError(
                    "Non terminals can't coexist with other rules!", getGenerator());
            }
        }

        constexpr auto checkItemExistence() const -> void
        {
            if (items.empty()) {
                throw DotItemParsingError(
                    "Unable to apply operation, because current item hasn't"
                    " been created!",
                    getGenerator());
            }
        }

        constexpr auto checkRuleOverloading() const -> void
        {
            constexpr ItemFlags repetition_rules =
                ItemFlags::PLUS | ItemFlags::STAR | ItemFlags::QUESTION;

            auto &last_item = items.back();
            auto last_item_flags = last_item->flags;

            if (last_item_flags.isAnyOfSet(repetition_rules)) {
                throw DotItemParsingError("Unable to apply more than one rule!", getGenerator());
            }
        }

    protected:
        text::GeneratorForText<CharT> rule_generator{};
        std::vector<item_ptr> items{};
        Allocator<CharT> allocator{ analysis_globals, items };
        size_t item_id{};
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct ItemParser<char>;
    extern template struct ItemParser<char8_t>;
    extern template struct ItemParser<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */
}// namespace cerb::lex

#endif /* CERBERUS_ITEM_PARSER_HPP */
