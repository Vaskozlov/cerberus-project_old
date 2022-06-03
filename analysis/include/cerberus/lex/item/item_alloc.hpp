#ifndef CERBERUS_ITEM_ALLOC_HPP
#define CERBERUS_ITEM_ALLOC_HPP

#include <cerberus/lex/item/basic_item.hpp>
#include <memory>

#define BASIC_ITEM_CHART cerb::lex::BasicItem<CharT>

namespace cerb::lex
{
    CERBERUS_EXCEPTION(AllocationError, CerberusException);

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

    template<typename ItemT, typename CharT>
    concept ItemObject = std::is_base_of_v<BASIC_ITEM_CHART, ItemT>;

    template<CharacterLiteral CharT>
    struct Allocator
    {
        using item_ptr = std::unique_ptr<BasicItem<CharT>>;

        template<typename... Ts>
        constexpr static auto newItemParser(
            AnalysisGlobals<CharT> &analysis_globals, std::vector<item_ptr> &items, Ts &&...args)
            -> ItemParser<CharT> *
        {
            return newItem<ItemParser<CharT>>(analysis_globals, items, std::forward<Ts>(args)...);
        }

        template<typename... Ts>
        constexpr static auto newRegex(
            AnalysisGlobals<CharT> &analysis_globals, std::vector<item_ptr> &items, Ts &&...args)
            -> regex::RegexItem<CharT> *
        {
            return newItem<regex::RegexItem<CharT>>(
                analysis_globals, items, std::forward<Ts>(args)...);
        }

        template<typename... Ts>
        constexpr static auto newString(
            AnalysisGlobals<CharT> &analysis_globals, std::vector<item_ptr> &items, Ts &&...args)
            -> string::StringItem<CharT> *
        {
            return newItem<string::StringItem<CharT>>(
                analysis_globals, items, std::forward<Ts>(args)...);
        }

        Allocator() = default;

    private:
        template<ItemObject<CharT> ItemT, typename... Ts>
        CERBLIB_DECL static auto newItem(
            AnalysisGlobals<CharT> &analysis_globals, std::vector<item_ptr> &items, Ts &&...args)
            -> ItemT *
        {
            item_ptr new_item =
                constructNewItem<ItemT>(analysis_globals, std::forward<Ts>(args)...);
            items.emplace_back(std::move(new_item));

            item_ptr &last_item = items.back();
            return convertSmartPointerToAnOrdinary<ItemT>(last_item);
        }

        template<ItemObject<CharT> ItemT>
        constexpr static auto convertSmartPointerToAnOrdinary(item_ptr &pointer) -> ItemT *
        {
            auto *casted_item = dynamic_cast<ItemT *>(pointer.get());
            checkThatPointerIsNotNull(casted_item);

            return casted_item;
        }

        template<ItemObject<CharT> ItemT>
        constexpr static auto checkThatPointerIsNotNull(ItemT const *item) -> void
        {
            if (item == nullptr) {
                throw AllocationError("Unable to allocate new item.");
            }
        }

        template<ItemObject<CharT> ItemT, typename... Ts>
        CERBLIB_DECL static auto
            constructNewItem(AnalysisGlobals<CharT> &analysis_globals, Ts &&...args) -> item_ptr
        {
            return std::make_unique<ItemT>(analysis_globals, std::forward<Ts>(args)...);
        }
    };
}// namespace cerb::lex

#undef BASIC_ITEM_CHART

#endif /* CERBERUS_ITEM_ALLOC_HPP */
