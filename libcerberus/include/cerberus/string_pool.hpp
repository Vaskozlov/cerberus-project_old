#ifndef CERBERUS_STRING_POOL_HPP
#define CERBERUS_STRING_POOL_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/string_view.hpp>
#include <map>

namespace cerb
{
    template<CharacterLiteral CharT, typename TokenType>
    struct StringPool
    {
        constexpr static u32 number_of_chars = pow2<u32>(bitsizeof(CharT));

        using str_view_t = BasicStringView<CharT>;
        using bitmap = ConstBitmap<1, number_of_chars>;

        using tokens_storage_t = std::map<str_view_t, TokenType>;
        using node_type = typename tokens_storage_t::node_type;
        using value_type = typename tokens_storage_t::value_type;

        using strings_storage_t = std::vector<bitmap>;
        using string_storage_const_iterator = typename strings_storage_t::const_iterator;

        constexpr auto insert(str_view_t &&string) -> decltype(auto)
        {
            addStringToBitmap(string);
            return tokens_by_strings.insert(std::move(string));
        }

        constexpr auto insert(str_view_t const &string) -> decltype(auto)
        {
            addStringToBitmap(string);
            return tokens_by_strings.insert(string);
        }

        template<typename... Ts>
        constexpr auto emplace(Ts &&...args) -> decltype(auto)
        {
            auto result = tokens_by_strings.template emplace<Ts...>(std::forward<Ts>(args)...);
            addStringToBitmap(result.first->first);
            return result;
        }

        constexpr auto insert(node_type &&node) -> decltype(auto)
        {
            addStringToBitmap(node.key());
            return tokens_by_strings.insert(std::move(node));
        }

        constexpr auto insert(node_type const &node) -> decltype(auto)
        {
            addStringToBitmap(node.key());
            return tokens_by_strings.insert(node);
        }

        constexpr auto operator[](str_view_t const &string) const -> TokenType
        {
            return tokens_by_strings.at(string);
        }

        CERBLIB_DECL auto findLongestStringInPool(str_view_t const &string) const -> str_view_t
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

            return { string.begin(), fetched_string_size };
        }

        CERBLIB_DECL auto contains(str_view_t const &string) const -> bool
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

        constexpr StringPool() = default;
        constexpr StringPool(std::initializer_list<value_type> const &nodes) : available_chars(4)
        {
            for (value_type const &node : nodes) {
                emplace(node);
            }
        }

    private:
        constexpr auto doesLevelContainChar(string_storage_const_iterator level, CharT chr) const
            -> bool
        {
            return std::bit_cast<bool>(level->template at<0>(convert2UnsignedInt(chr)));
        }

        constexpr auto addStringToBitmap(str_view_t const &string)
        {
            resizeIfNeed(string);
            auto level = available_chars.begin();

            for (CharT chr : string) {
                level->template set<1, 0>(convert2UnsignedInt(chr));
                ++level;
            }
        }

        constexpr auto resizeIfNeed(str_view_t const &string)
        {
            if (string.size() > available_chars.size()) {
                available_chars.resize(string.size());
            }
        }

        strings_storage_t available_chars{};
        tokens_storage_t tokens_by_strings{};
    };
}// namespace cerb

#endif /* CERBERUS_STRING_POOL_HPP */
