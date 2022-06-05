#ifndef CERBERUS_ITEM_HPP
#define CERBERUS_ITEM_HPP

#include <cerberus/lex/item/errros/item_parser_errors.hpp>
#include <cerberus/lex/item/item_alloc.hpp>
#include <cerberus/lex/item/regex.hpp>
#include <cerberus/lex/item/string.hpp>
#include <cerberus/text/bracket_finder.hpp>
#include <utility>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct DotItem
      : public BasicItem<CharT>
      , private text::ScanApi<true, CharT>
    {
        CERBLIB_BASIC_ITEM_ACCESS(CharT);
        CERBLIB_SCAN_API_ACCESS(true, CharT);

        friend DotItemErrors<CharT>;

        using Error = DotItemErrors<CharT>;
        using item_ptr = std::unique_ptr<BasicItem<CharT>>;

        CERBLIB_DECL auto getId() const -> size_t
        {
            return item_id;
        }

        CERBLIB_DECL auto getItems() const -> SmallVector<item_ptr> const &
        {
            return items;
        }

        CERBLIB_DECL auto scan() const -> ScanResult override
        {
            return ScanResult{};
        }

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
          : CERBLIB_CONSTRUCT_BASIC_ITEM, scan_api_t(rule_generator), rule_generator(rule),
            item_id(id_of_item)
        {
            scan_api_t::beginScanning(CharEnum<CharT>::EoF);
        }

        constexpr DotItem(
            AnalysisGlobals<CharT> &analysis_parameters, size_t id_of_item,
            text::GeneratorForText<CharT> const &gen)
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
            Error::checkItemIsNotNonterminal(*this);

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
                addDotItem();
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
                Error::mistakeInRegex(*this);
            }
        }

        constexpr auto onEnd() -> void override
        {
            Error::checkItemNotEmpty(*this);
        }

        constexpr auto postInitializationSetup() -> void override
        {
            if (flags.isSet(ItemFlags::REVERSE)) {
                std::ranges::reverse(items);
            }
        }

        constexpr auto setTag(ItemFlags new_tag) -> void
        {
            Error::checkItemExistence(*this);

            item_ptr &last_item = items.back();
            last_item->flags |= new_tag;
        }

        constexpr auto setRepetitionRule(ItemFlags new_rule) -> void
        {
            Error::checkItemExistence(*this);
            Error::checkRuleOverloading(*this);

            item_ptr &last_item = items.back();
            last_item->flags |= new_rule;
        }

        constexpr auto addString() -> void
        {
            Allocator<CharT>::newString(analysis_globals, items, rule_generator);
        }

        constexpr auto addDotItem() -> void
        {
            constexpr size_t begin_item_length = cerb::strlen("(");

            size_t item_length = getItemLength();
            text::GeneratorForText<CharT> forked_gen =
                rule_generator.fork(begin_item_length, item_length);
            auto *new_item =
                Allocator<CharT>::newDotItem(analysis_globals, items, getId(), forked_gen);

            Error::checkItemIsNotNonterminal(*new_item);
            skipItemBorder(item_length);
        }

        constexpr auto addRegex() -> void
        {
            Allocator<CharT>::newRegex(analysis_globals, items, rule_generator);
        }

        constexpr auto addNonTerminal() -> void
        {
            Error::checkThatNonTerminalCanBeAdded(*this);

            std::basic_string<CharT> converted_str =
                convertStringToCodes(cast('\''), rule_generator);
            makeNonterminalGlobal(std::move(converted_str));

            flags |= ItemFlags::NONTERMINAL;
        }

        constexpr auto skipItemBorder(size_t border) -> void
        {
            scan_api_t::skip(border);
        }

        CERBLIB_DECL auto getItemLength() const -> size_t
        {
            return findBracket(cast('('), cast(')'), getGenerator());
        }

        constexpr auto completeLastItem() -> void
        {
            if (not items.empty()) {
                item_ptr &last_item = items.back();
                last_item->postInitializationSetup();
            }
        }

        constexpr auto makeNonterminalGlobal(std::basic_string<CharT> &&str) -> void
        {
            analysis_globals.emplaceNonterminal(std::move(str), getId());
        }

        text::GeneratorForText<CharT> rule_generator{};
        SmallVector<item_ptr> items{};
        size_t item_id{};
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct DotItem<char>;
    extern template struct DotItem<char8_t>;
    extern template struct DotItem<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::lex

#endif /* CERBERUS_ITEM_HPP */
