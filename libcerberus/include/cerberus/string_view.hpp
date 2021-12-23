#ifndef CERBERUS_STRING_VIEW_HPP
#define CERBERUS_STRING_VIEW_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/memory.hpp>
#include <cerberus/type.hpp>
#include <string_view>

namespace cerb {
    template<CharacterLiteral CharT>
    struct basic_string_view
    {
        using size_type              = usize;
        using difference_type        = ptrdiff_t;
        using value_type             = CharT;
        using pointer                = value_type *;
        using const_pointer          = const value_type *;
        using reference              = value_type &;
        using const_reference        = const value_type &;
        using const_iterator         = const CharT *;
        using iterator               = const_iterator;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        size_type length{};
        const CharT *string{};

    public:
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
            return cerb::find(string, chr, length);
        }

        CERBLIB_DECL auto find(CharT chr, size_t position) const -> size_t
        {
            return cerb::find(string + position, chr, length - position);
        }

        CERBLIB_DECL auto str_view() const -> std::basic_string_view<CharT>
        {
            return { string, length };
        }

        CERBLIB_DECL auto str() const -> std::basic_string<CharT>
        {
            return std::basic_string<CharT>(str_view());
        }

        CERBLIB_DECL auto substr(size_t from, size_t size = std::numeric_limits<u32>::max()) const
            -> basic_string_view
        {
            return { cbegin() + from, max<size_t>(size, length - from) };
        }

        template<typename T>
        CERBLIB_DECL auto operator==(const T &other) const -> bool
        {
            static_assert(is_any_of_v<
                          T, basic_string_view<CharT>, std::basic_string<CharT>,
                          std::basic_string_view<CharT>>);

            return cerb::areObjectsInClassEqual(
                *this, basic_string_view<CharT>(std::data(other), std::size(other)));
        }

        template<typename T>
        CERBLIB_DECL auto operator<=>(const T &other) const -> std::strong_ordering
        {
            static_assert(is_any_of_v<
                          T, basic_string_view<CharT>, std::basic_string<CharT>,
                          std::basic_string_view<CharT>>);

            for (size_t i = 0; i < min<size_type>(this->length, std::size(other)); ++i) {
                if (this->at(i) != other.at(i)) {
                    return this->at(i) <=> other.at(i);
                }
            }

            return this->length <=> std::size(other);
        }

        constexpr basic_string_view() = default;

        constexpr basic_string_view(const CharT *str) : length(strlen(str)), string(str)
        {}

        constexpr basic_string_view(const CharT *str, usize lengthOfString)
          : length(lengthOfString), string(str)
        {}

        template<unsigned long Size>
        constexpr basic_string_view(const CharT (&str)[Size]) : length(Size), string(str)
        {}
    };

    using string_view    = cerb::basic_string_view<char>;
    using u8string_view  = cerb::basic_string_view<char8_t>;
    using u16string_view = cerb::basic_string_view<char16_t>;
    using u32string_view = cerb::basic_string_view<char32_t>;
    using wstring_view   = cerb::basic_string_view<wchar_t>;

    namespace string_view_literals {
        consteval auto operator""_sv(const char *string, size_t length)
            -> cerb::basic_string_view<char>
        {
            return { string, length };
        }

        consteval auto operator""_sv(const char8_t *string, size_t length)
            -> cerb::basic_string_view<char8_t>
        {
            return { string, length };
        }

        consteval auto operator""_sv(const char16_t *string, size_t length)
            -> cerb::basic_string_view<char16_t>
        {
            return { string, length };
        }

        consteval auto operator""_sv(const char32_t *string, size_t length)
            -> cerb::basic_string_view<char32_t>
        {
            return { string, length };
        }

        consteval auto operator""_sv(const wchar_t *string, size_t length)
            -> cerb::basic_string_view<wchar_t>
        {
            return { string, length };
        }
    }// namespace string_view_literals
}// namespace cerb

template<typename OsType, cerb::CharacterLiteral CharT>
auto operator<<(OsType &os, const cerb::basic_string_view<CharT> &string) -> OsType &
{
    return os << string.str_view();
}

#endif /* CERBERUS_STRING_VIEW_HPP */
