#ifndef CERBERUS_STRING_CONTAINER_HPP
#define CERBERUS_STRING_CONTAINER_HPP

#include <cerberus/const_bitmap.hpp>
#include <cerberus/number.hpp>
#include <cerberus/string_view.hpp>
#include <map>
#include <vector>

namespace cerb
{
    template<CharacterLiteral CharT, typename TokenType, bool UseStdString = false>
    struct StringContainer
    {
        constexpr static size_t number_of_chars = pow2<size_t>(bitsizeof(CharT));

        using str_t =
            std::conditional_t<UseStdString, std::basic_string<CharT>, BasicStringView<CharT>>;
        using tokens_storage_t = std::map<str_t, TokenType>;
        using strings_storage_t = std::vector<ConstBitmap<1, number_of_chars>>;

        using node_type = typename tokens_storage_t::node_type;
        using value_type = typename tokens_storage_t::value_type;
        using tokens_storage_iterator = typename tokens_storage_t::iterator;
        using string_storage_const_iterator = typename strings_storage_t::const_iterator;
        using tokens_storage_insert_return_type = typename tokens_storage_t::insert_return_type;

        constexpr auto insert(str_t const &string) -> std::pair<tokens_storage_iterator, bool>
        {
            addStringToBitmap(string);
            return tokens_by_strings.insert(string);
        }

        template<typename... Ts>
        constexpr auto emplace(Ts &&...args) -> std::pair<tokens_storage_iterator, bool>
        {
            auto inserted_item =
                tokens_by_strings.template emplace<Ts...>(std::forward<Ts>(args)...);
            addStringToBitmap(inserted_item.first->first);
            return inserted_item;
        }

        constexpr auto insert(node_type &&node) -> tokens_storage_insert_return_type
        {
            addStringToBitmap(node.key());
            return tokens_by_strings.insert(std::move(node));
        }

        CERBLIB_DECL auto operator[](str_t const &string) const -> TokenType
        {
            if (not tokens_by_strings.contains(string)) {
                throw std::out_of_range("Cerberus string pool does not contains given element!");
            }

            return tokens_by_strings.at(string);
        }

        template<StringType<CharT> Str>
        CERBLIB_DECL auto findLongestString(Str const &string) const -> Str
        {
            size_t fetched_string_size = 0;
            auto level = available_chars.begin();

            for (CharT chr : string) {
                if (level == available_chars.end() || not doesLevelContainChar(level, chr)) {
                    break;
                }

                ++level;
                ++fetched_string_size;
            }

            return { string.begin(), string.begin() + fetched_string_size };
        }

        template<StringType<CharT> Str>
        CERBLIB_DECL auto contains(Str const &string) const -> bool
        {
            auto level = available_chars.begin();

            for (CharT chr : string) {
                if (level == available_chars.end() || not doesLevelContainChar(level, chr)) {
                    return false;
                }
                ++level;
            }

            return true;
        }

        StringContainer() = default;

        constexpr StringContainer(std::initializer_list<value_type> const &nodes)
          : available_chars(4)
        {
            auto emplace_node = [this](value_type const &node) { this->emplace(node); };
            std::ranges::for_each(nodes, emplace_node);
        }

    private:
        CERBLIB_DECL auto doesLevelContainChar(string_storage_const_iterator level, CharT chr) const
            -> bool
        {
            return level->template at<0>(asUInt(chr));
        }

        constexpr auto addStringToBitmap(str_t const &string) -> void
        {
            resizeIfNeed(string);

            auto level = available_chars.begin();
            auto set_level = [&level](CharT chr) {
                level->template set<1, 0>(asUInt(chr));
                ++level;
            };

            std::ranges::for_each(string, set_level);
        }

        constexpr auto resizeIfNeed(str_t const &string) -> void
        {
            auto length_of_string = string.size();
            auto length_of_chars_storage = available_chars.size();

            if (length_of_string > length_of_chars_storage) {
                available_chars.resize(string.size());
            }
        }

        strings_storage_t available_chars{};
        tokens_storage_t tokens_by_strings{};
    };
}// namespace cerb

#endif /* CERBERUS_STRING_CONTAINER_HPP */
