#ifndef CERBERUS_RANGE_HPP
#define CERBERUS_RANGE_HPP

#include <cerberus/cerberus.hpp>
#include <concepts>
#include <iterator>

namespace cerb
{
    template<std::integral Int>
    struct Range
    {
        struct iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;

        CERBLIB_DECL auto begin() const -> iterator
        {
            return iterator{ begin_of_range, increment };
        }

        CERBLIB_DECL auto end() const -> iterator
        {
            return iterator{ end_of_range, increment };
        }

        CERBLIB_DECL auto cbegin() const -> iterator
        {
            return iterator{ begin_of_range, increment };
        }

        CERBLIB_DECL auto cend() const -> iterator
        {
            return iterator{ end_of_range, increment };
        }

        CERBLIB_DECL auto rbegin() const -> reverse_iterator
        {
            return reverse_iterator(begin());
        }

        CERBLIB_DECL auto rend() const -> reverse_iterator
        {
            return reverse_iterator(end());
        }

        CERBLIB_DECL auto crbegin() const -> reverse_iterator
        {
            return reverse_iterator(begin());
        }

        CERBLIB_DECL auto crend() const -> reverse_iterator
        {
            return reverse_iterator(end());
        }

        Range() = default;

        constexpr explicit Range(Int to) : end_of_range(to)
        {}

        constexpr Range(Int from, Int to) : begin_of_range(from), end_of_range(to)
        {}

        constexpr Range(Int from, Int to, Int inc)
          : begin_of_range(from), end_of_range(to), increment(inc)
        {}

    private:
        Int begin_of_range{ 0 };
        Int end_of_range{ 0 };
        Int increment{ 1 };
    };

    template<std::integral Int>
    struct Range<Int>::iterator
    {
        using value_type = Int;
        using pointer = Int *;
        using refernce = Int &;
        using difference_type = ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

        CERBLIB_DECL auto operator*() const -> Int
        {
            return state;
        }

        constexpr auto operator++() -> iterator &
        {
            state += increment;
            return *this;
        }

        constexpr auto operator++(int) -> iterator
        {
            auto save = *this;
            state += increment;
            return this;
        }

        constexpr auto operator--() -> iterator &
        {
            state -= increment;
            return *this;
        }

        constexpr auto operator--(int) -> iterator
        {
            auto save = *this;
            state -= increment;
            return this;
        }

        CERBLIB_DECL auto operator+(size_t rhs) -> iterator &
        {
            state += rhs * increment;
            return *this;
        }

        CERBLIB_DECL auto operator-(size_t rhs) -> iterator &
        {
            state -= rhs * increment;
            return *this;
        }

        CERBLIB_DECL auto operator+(iterator const &rhs) const -> ptrdiff_t
        {
            return static_cast<ptrdiff_t>(state) + static_cast<ptrdiff_t>(rhs.state);
        }

        CERBLIB_DECL auto operator-(iterator const &rhs) const -> ptrdiff_t
        {
            return static_cast<ptrdiff_t>(state) - static_cast<ptrdiff_t>(rhs.state);
        }

        CERBLIB_DECL auto operator!=(iterator const &other) const -> bool
        {
            if (increment > 0) {
                return state < other.state;
            }

            return state > other.state;
        }

        iterator() = default;

        constexpr iterator(Int current_state, Int inc) : state(current_state), increment(inc)
        {}

    private:
        Int state{};
        Int increment{};
    };
}// namespace cerb

#endif /* CERBERUS_RANGE_HPP */
