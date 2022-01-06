#ifndef CERBERUS_STRING_VIEW_HPP
#define CERBERUS_STRING_VIEW_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/memory.hpp>
#include <cerberus/type.hpp>
#include <string_view>

namespace cerb {
    template<CharacterLiteral CharT>
    struct BasicStringView
    {
        using size_type = usize;
        using difference_type = ptrdiff_t;
        using value_type = CharT;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using const_iterator = const CharT *;
        using iterator = const_iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        CERBLIB_DECL auto size() const -> usize
        {
            return length;
        }

        CERBLIB_DECL auto data() const -> const CharT *
        {
            return string;
        }

        CERBLIB_DECL auto begin() const -> const_iterator
        {
            return string;
        }

        CERBLIB_DECL auto end() const -> const_iterator
        {
            return string + length;
        }

        CERBLIB_DECL auto cbegin() const -> const_iterator
        {
            return begin();
        }

        CERBLIB_DECL auto cend() const -> const_iterator
        {
            return end();
        }

        CERBLIB_DECL auto rbegin() const -> const_reverse_iterator
        {
            return const_reverse_iterator(begin());
        }

        CERBLIB_DECL auto rend() const -> const_reverse_iterator
        {
            return const_reverse_iterator(end());
        }

        CERBLIB_DECL auto crbegin() const -> const_reverse_iterator
        {
            return const_reverse_iterator(begin());
        }

        CERBLIB_DECL auto crend() const -> const_reverse_iterator
        {
            return const_reverse_iterator(end());
        }

        CERBLIB_DECL auto front() const -> CharT
        {
            return *begin();
        }

        CERBLIB_DECL auto back() const -> CharT
        {
            return *(begin() + length - 1);
        }

        CERBLIB_DECL auto at(size_type index) const -> CharT
        {
            return string[index];
        }

        CERBLIB_DECL auto operator[](size_type index) const -> CharT
        {
            return string[index];
        }

        CERBLIB_DECL auto find(CharT chr) const -> size_t
        {
            return ptrdiff(begin(), cerb::find(*this, chr));
        }

        CERBLIB_DECL auto find(CharT chr, size_t position) const -> size_t
        {
            return ptrdiff(begin(), cerb::find(string + position, chr, length - position));
        }

        CERBLIB_DECL auto strView() const -> std::basic_string_view<CharT>
        {
            return { string, length };
        }

        CERBLIB_DECL auto str() const -> std::basic_string<CharT>
        {
            return std::basic_string<CharT>(strView());
        }

        CERBLIB_DECL auto substr(size_t from, size_t size = std::numeric_limits<u32>::max()) const
            -> BasicStringView
        {
            return { cbegin() + from, max<size_t>(size, length - from) };
        }

        CERBLIB_DECL auto containsAt(size_t index, BasicStringView const &str) -> bool
        {
            return BasicStringView(begin() + index, str.size()).operator==(str);
        }

        CERBLIB_DECL auto operator==(const CharT *other) const -> bool
        {
            return this->operator==(BasicStringView<CharT>(other));
        }

        template<typename T>
        CERBLIB_DECL auto operator==(const T &other) const -> bool
        {
            static_assert(is_any_of_v<
                          T, BasicStringView<CharT>, std::basic_string<CharT>,
                          std::basic_string_view<CharT>>);

            return std::ranges::equal(
                *this, BasicStringView<CharT>(std::data(other), std::size(other)));
        }

        CERBLIB_DECL auto operator<=>(const CharT *other) const -> decltype(auto)
        {
            return this->operator<=>(BasicStringView<CharT>(other));
        }

        template<typename T>
        CERBLIB_DECL auto operator<=>(const T &other) const
        {
            static_assert(is_any_of_v<
                          T, BasicStringView<CharT>, std::basic_string<CharT>,
                          std::basic_string_view<CharT>>);

            for (size_t i = 0; i < min<size_type>(this->length, std::size(other)); ++i) {
                if (this->at(i) != other.at(i)) {
                    return this->at(i) <=> other.at(i);
                }
            }

            return this->length <=> std::size(other);
        }

        constexpr BasicStringView() = default;

        constexpr BasicStringView(const CharT *str) : length(strlen(str)), string(str)
        {}

        constexpr BasicStringView(const CharT *str, usize length_of_string)
          : length(length_of_string), string(str)
        {}

        constexpr BasicStringView(iterator first, iterator last)
          : length(ptrdiff(first, last)), string(first)
        {}

        template<unsigned long Size>
        constexpr BasicStringView(const CharT (&str)[Size]) : length(Size), string(str)
        {}

    private:
        size_type length{};
        const CharT *string{};
    };

    using string_view = BasicStringView<char>;
    using u8string_view = BasicStringView<char8_t>;
    using u16string_view = BasicStringView<char16_t>;
    using u32string_view = BasicStringView<char32_t>;
    using wstring_view = BasicStringView<wchar_t>;

    namespace string_view_literals {
        consteval auto operator""_sv(const char *string, size_t length) -> BasicStringView<char>
        {
            return { string, length };
        }

        consteval auto operator""_sv(const char8_t *string, size_t length)
            -> BasicStringView<char8_t>
        {
            return { string, length };
        }

        consteval auto operator""_sv(const char16_t *string, size_t length)
            -> BasicStringView<char16_t>
        {
            return { string, length };
        }

        consteval auto operator""_sv(const char32_t *string, size_t length)
            -> BasicStringView<char32_t>
        {
            return { string, length };
        }

        consteval auto operator""_sv(const wchar_t *string, size_t length)
            -> BasicStringView<wchar_t>
        {
            return { string, length };
        }
    }// namespace string_view_literals
}// namespace cerb

template<typename OsType, cerb::CharacterLiteral CharT>
auto operator<<(OsType &os, const cerb::BasicStringView<CharT> &string) -> OsType &
{
    return os << string.strView();
}

#endif /* CERBERUS_STRING_VIEW_HPP */
