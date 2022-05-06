#ifndef CERBERUS_ITEM_PARSER_HPP
#define CERBERUS_ITEM_PARSER_HPP

#include <cerberus/lex/bracket_finder.hpp>
#include <cerberus/lex/item/regex.hpp>
#include <cerberus/lex/item/string.hpp>

#define CERBLIB_ITEM_PARSER_ACCESS(CharT)                                                          \
    using item_parser_t = cerb::lex::ItemParser<CharT>;                                            \
    using typename item_parser_t::item_ptr;                                                        \
    using item_parser_t::id;                                                                       \
    using item_parser_t::cast;                                                                     \
    using item_parser_t::flags;                                                                    \
    using item_parser_t::item_id;                                                                  \
    using item_parser_t::items;                                                                    \
    using item_parser_t::recent_item;                                                              \
    using item_parser_t::rule_generator;                                                           \
    using item_parser_t::analysis_globals

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct ItemParser : public BasicItem<CharT>
    {
        CERBLIB_BASIC_ITEM_ACCESS(CharT);
        using item_ptr = std::unique_ptr<BasicItem<CharT>>;

        CERBERUS_ANALYSIS_EXCEPTION(DotItemParsingError, CharT, BasicLexicalAnalysisException);

        CERBLIB_DECL auto id() const -> size_t
        {
            return item_id;
        }

        CERBLIB_DECL auto getItems() const -> std::vector<item_ptr> const &
        {
            return items;
        }

        ItemParser() = default;

        constexpr ItemParser(
            CERBLIB_BASIC_ITEM_ARGS,
            size_t id_of_item,
            BasicStringView<CharT> const &rule)
          : CERBLIB_CONSTRUCT_BASIC_ITEM, rule_generator(rule), item_id(id_of_item)
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
                throw DotItemParsingError(
                    "Error in regex during the rule parsing!", rule_generator);
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
        {
            BasicStringView<CharT> text = rule_generator.getRestOfTheText();
            size_t border = getBorder(rule_generator);

            text = extractTextForNewItem(text, border);
            BasicItem<CharT> *new_item = createNewItem<ItemParser<CharT>>(id(), text);

            checkItemNonEmpty(asItemParser(new_item));
            skipItemBorder(border);
        }

        constexpr auto addRegex() -> void
        {
            createNewItem<regex::RegexItem<CharT>>(rule_generator);
        }

        constexpr auto addNonTerminal() -> void
        {
            checkThatNonTerminalCanBeAdded();

            std::basic_string<CharT> converted_str =
                convertStringToCodes(cast('\''), rule_generator);
            makeNonterminalGlobal(std::move(converted_str));

            flags |= ItemFlags::NONTERMINAL;
        }

        CERBLIB_DECL static auto asItemParser(BasicItem<CharT> *item) -> ItemParser<CharT> *
        {
            return dynamic_cast<ItemParser<CharT> *>(item);
        }

        CERBLIB_DECL static auto asStringItem(BasicItem<CharT> *item) -> string::StringItem<CharT> *
        {
            return dynamic_cast<string::StringItem<CharT> *>(item);
        }

        constexpr auto skipItemBorder(size_t border) -> void
        {
            rule_generator.skip(border);
        }

        CERBLIB_DECL static auto
            extractTextForNewItem(BasicStringView<CharT> const &text, size_t border)
                -> BasicStringView<CharT>
        {
            constexpr size_t length_of_item_begin = 1;
            constexpr size_t length_of_item_end = 1;

            auto new_begin = text.begin() + length_of_item_begin;
            auto new_length = border - length_of_item_end;

            return { new_begin, new_length };
        }

        CERBLIB_DECL static auto getBorder(GeneratorForText<CharT> const &gen) -> size_t
        {
            return findBracket(cast('('), cast(')'), gen);
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
        CERBLIB_DECL auto allocateNewItem(Ts &&...args) -> item_ptr
        {
            static_assert(std::is_base_of_v<BasicItem<CharT>, T>);

            return std::make_unique<T>(analysis_globals, std::forward<Ts>(args)...);
        }

        constexpr auto makeNonterminalGlobal(std::basic_string<CharT> &&str) -> void
        {
            analysis_globals.emplaceNonterminal(std::move(str), id());
        }

        constexpr auto checkItemNonEmpty(ItemParser<CharT> const *item_to_check) const -> void
        {
            if (item_to_check->items.empty()) {
                throw DotItemParsingError("Empty items are not allowed!", rule_generator);
            }
        }

        constexpr auto checkForAlreadyExistsNonterminal() const -> void
        {
            if (flags.isSet(ItemFlags::NONTERMINAL)) {
                throw DotItemParsingError(
                    "Nonterminals can't coexist with other rules!", rule_generator);
            }
        }

        constexpr auto checkThatNonTerminalCanBeAdded() const -> void
        {
            if (recent_item != nullptr) {
                throw DotItemParsingError(
                    "Non terminals can't coexist with other rules!", rule_generator);
            }
        }

        constexpr auto checkItemExistence() const -> void
        {
            if (recent_item == nullptr) {
                throw DotItemParsingError(
                    "Unable to apply operation, because current item hasn't"
                    " been created!",
                    rule_generator);
            }
        }

        constexpr auto checkRuleOverloading() const -> void
        {
            constexpr ItemFlags repetition_rules =
                ItemFlags::PLUS | ItemFlags::STAR | ItemFlags::QUESTION;

            if ((recent_item->flags & repetition_rules) != ItemFlags::NONE) {
                throw DotItemParsingError("Unable to apply more than one rule!", rule_generator);
            }
        }

    protected:
        // NOLINTBEGIN
        GeneratorForText<CharT> rule_generator{};
        std::vector<item_ptr> items{};
        BasicItem<CharT> *recent_item{};
        size_t item_id{};
        // NOLINTEND
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct ItemParser<char>;
    extern template struct ItemParser<char8_t>;
    extern template struct ItemParser<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */
}// namespace cerb::lex

#endif /* CERBERUS_ITEM_PARSER_HPP */
