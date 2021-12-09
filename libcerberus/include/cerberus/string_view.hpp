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
            return *(end() - 1);
        }

        CERBLIB_DECL auto at(size_type index) const -> const_reverse_iterator
        {
            return string[index];
        }

        CERBLIB_DECL auto operator[](size_type index) const -> const_reverse_iterator
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
}// namespace cerb

#endif /* CERBERUS_STRING_VIEW_HPP */
