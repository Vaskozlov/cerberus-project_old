#ifndef CERBERUS_STRING_POOL_HPP
#define CERBERUS_STRING_POOL_HPP

#include <cerberus/string_view.hpp>
#include <map>
#include <memory_resource>
#include <utility>

namespace cerb {
    template<CharacterLiteral CharT, typename TokenType, size_t BufferSize = 1024>
    class StringPool
    {
        using value_type = std::pair<const BasicStringView<CharT>, TokenType>;

    public:
        constexpr auto addString(value_type const &new_value)
        {
            strings.emplace(new_value.first, new_value.second);
        }

        constexpr auto contains(BasicStringView<CharT> const &str) -> bool
        {
            return strings.contains(str);
        }

        constexpr auto getTokenOfString(BasicStringView<CharT> const &str) -> TokenType
        {
            return strings.at(str);
        }

        constexpr StringPool() = default;
        constexpr StringPool(std::initializer_list<value_type> const &strings2store)
          : strings(strings2store, &resource)
        {}

    private:
        std::array<u8, BufferSize> string_buffer{};
        std::pmr::monotonic_buffer_resource resource{ string_buffer.data(), string_buffer.size() };
        std::pmr::map<BasicStringView<CharT>, TokenType> strings{ &resource };
    };
}// namespace cerb

#endif /* CERBERUS_STRING_POOL_HPP */
