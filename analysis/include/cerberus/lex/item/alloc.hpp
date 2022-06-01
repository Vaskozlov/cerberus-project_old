#ifndef CERBERUS_ALLOC_HPP
#define CERBERUS_ALLOC_HPP

#include <cerberus/lex/item/basic_item.hpp>
#include <memory>

namespace cerb::lex
{
    namespace regex
    {
        template<CharacterLiteral CharT>
        struct RegexItem;
    }

    namespace string
    {
        template<CharacterLiteral CharT>
        struct StringItem;
    }

    template<CharacterLiteral CharT>
    struct ItemParser;

    template<typename T, typename CharT>
    concept ItemObject = IsAnyOfV < T,
            ItemParser<CharT>,
    string::StringItem<CharT>, regex::RegexItem < CharT >> ;

    template<CharacterLiteral CharT>
    struct Allocator
    {
        using item_ptr = std::unique_ptr<BasicItem<CharT>>;

        template<typename... Ts>
        constexpr auto newItemParser(Ts &&...args) -> ItemParser<CharT> *
        {
            return newItem<ItemParser<CharT>>(std::forward<Ts>(args)...);
        }

        template<typename... Ts>
        constexpr auto newRegex(Ts &&...args) -> regex::RegexItem<CharT> *
        {
            return newItem<regex::RegexItem<CharT>>(std::forward<Ts>(args)...);
        }

        template<typename... Ts>
        constexpr auto newString(Ts &&...args) -> string::StringItem<CharT> *
        {
            return newItem<string::StringItem<CharT>>(std::forward<Ts>(args)...);
        }

        Allocator() = default;

        Allocator(AnalysisGlobals<CharT> &globals, std::vector<item_ptr> &items_storage)
          : analysis_globals(globals), items(items_storage)
        {}

    private:
        template<ItemObject<CharT> ItemT, typename... Ts>
        constexpr auto newItem(Ts &&...args) -> ItemT *
        {
            item_ptr new_item = constructNewItem<ItemT>(std::forward<Ts>(args)...);
            items.emplace_back(std::move(new_item));

            item_ptr &last_item = items.back();
            return dynamic_cast<ItemT *>(last_item.get());
        }

        template<ItemObject<CharT> ItemT, typename... Ts>
        CERBLIB_DECL auto constructNewItem(Ts &&...args) -> item_ptr
        {
            return std::make_unique<ItemT>(analysis_globals, std::forward<Ts>(args)...);
        }

        AnalysisGlobals<CharT> &analysis_globals;
        std::vector<item_ptr> &items;
    };
}// namespace cerb::lex

#endif /* CERBERUS_ALLOC_HPP */
