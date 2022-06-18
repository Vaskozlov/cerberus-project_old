#ifndef CERBERUS_STRING_VIEW_HPP
#define CERBERUS_STRING_VIEW_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/memory.hpp>
#include <cerberus/number.hpp>
#include <cerberus/range.hpp>
#include <cerberus/type.hpp>
#include <string_view>

namespace cerb
{
    template<CharacterLiteral T>
    struct BasicStringView;

    template<typename T, typename CharT>
    concept StringType =
        CharacterLiteral<CharT> &&
        IsAnyOfV<
            T, BasicStringView<CharT>, std::basic_string<CharT>, std::basic_string_view<CharT>>;

    template<CharacterLiteral CharT>
    struct CERBLIB_TRIVIAL BasicStringView
    {
        using value_type = CharT;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        using const_iterator = CharT const *;
        using iterator = const_iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        CERBLIB_DECL auto size() const -> size_t
        {
            return length;
        }

        CERBLIB_DECL auto empty() const -> bool
        {
            return size() == 0;
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
            return const_reverse_iterator(end());
        }

        CERBLIB_DECL auto rend() const -> const_reverse_iterator
        {
            return const_reverse_iterator(begin());
        }

        CERBLIB_DECL auto crbegin() const -> const_reverse_iterator
        {
            return const_reverse_iterator(end());
        }

        CERBLIB_DECL auto crend() const -> const_reverse_iterator
        {
            return const_reverse_iterator(begin());
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
            auto index = ptrdiff(begin(), cerb::find(*this, chr));
            return index < length ? index : npos;
        }

        CERBLIB_DECL auto find(CharT chr, size_t position) const -> size_t
        {
            return ptrdiff(begin(), cerb::find(string + position, chr, length - position));
        }

        CERBLIB_DECL auto rfind(CharT chr) const -> size_t
        {
            return ptrdiff(cerb::rfind(*this, chr), rend()) - 1;
        }

        CERBLIB_DECL auto rfind2(CharT chr) const
        {
            return cerb::rfind(*this, chr);
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

        CERBLIB_DECL auto containsAt(size_t index, BasicStringView const &str) const -> bool
        {
            if (index + str.size() > size()) {
                return false;
            }

            return BasicStringView(begin() + index, str.size()).operator==(str);
        }

        CERBLIB_DECL auto operator==(CharT const *other) const -> bool
        {
            return operator==(BasicStringView<CharT>(other));
        }

        template<StringType<CharT> T>
        CERBLIB_DECL auto operator==(T const &other) const -> bool
        {
            auto other_str = BasicStringView<CharT>(std::data(other), std::size(other));
            return std::ranges::equal(*this, other_str);
        }

        CERBLIB_DECL auto operator<=>(CharT const *other) const -> decltype(auto)
        {
            return operator<=>(BasicStringView<CharT>(other));
        }

        template<StringType<CharT> T>
        CERBLIB_DECL auto operator<=>(T const &other) const
        {
            size_t maximum_checking_length = min<size_type>(length, std::size(other));

            CERBLIB_UNROLL_N(4)
            for (size_t i = 0; i != maximum_checking_length; ++i) {
                if (at(i) != other.at(i)) {
                    return at(i) <=> other.at(i);
                }
            }

            return length <=> std::size(other);
        }

        BasicStringView() = default;

        template<ArrayOrPointer T>// NOLINTNEXTLINE
        constexpr BasicStringView(T const &str) : length(strlen(str)), string(str)
        {}

        constexpr BasicStringView(CharT const *str, size_t length_of_string)
          : length(length_of_string), string(str)
        {}

        constexpr BasicStringView(iterator first, iterator last)
          : length(ptrdiff(first, last)), string(first)
        {}

        template<StringType<CharT> T>// NOLINTNEXTLINE
        constexpr BasicStringView(T const &str) : length(std::size(str)), string(std::data(str))
        {}

        // NOLINTNEXTLINE
        constexpr static size_t npos = std::numeric_limits<size_t>::max();

    private:
        size_type length{};
        CharT const *string{};
    };

    using string_view = BasicStringView<char>;
    using u8string_view = BasicStringView<char8_t>;
    using u16string_view = BasicStringView<char16_t>;
    using u32string_view = BasicStringView<char32_t>;
    using wstring_view = BasicStringView<wchar_t>;

    namespace string_view_literals
    {
        constexpr auto operator""_sv(char const *string, size_t length) -> BasicStringView<char>
        {
            return { string, length };
        }

        constexpr auto operator""_sv(char8_t const *string, size_t length)
            -> BasicStringView<char8_t>
        {
            return { string, length };
        }

        constexpr auto operator""_sv(char16_t const *string, size_t length)
            -> BasicStringView<char16_t>
        {
            return { string, length };
        }

        constexpr auto operator""_sv(char32_t const *string, size_t length)
            -> BasicStringView<char32_t>
        {
            return { string, length };
        }

        constexpr auto operator""_sv(wchar_t const *string, size_t length)
            -> BasicStringView<wchar_t>
        {
            return { string, length };
        }
    }// namespace string_view_literals
}// namespace cerb

template<typename OsType, cerb::CharacterLiteral CharT>
auto operator<<(OsType &os, cerb::BasicStringView<CharT> const &string) -> OsType &
{
    return os << string.strView();
}

#endif /* CERBERUS_STRING_VIEW_HPP */
