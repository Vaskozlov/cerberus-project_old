#ifndef LIBCERBERUS_PAIR_HPP
#define LIBCERBERUS_PAIR_HPP

#include <cerberus/bits.hpp>
#include <cerberus/cerberus.hpp>

namespace cerb {
    enum struct HowToComparePair
    {
        DEFAULT,
        BY_FIRST_VALUE,
        BY_SECOND_VALUE
    };

    template<typename T1, typename T2, HowToComparePair ComparisonRule = HowToComparePair::DEFAULT>
    struct CERBLIB_TRIVIAL Pair
    {
        T1 first{};
        T2 second{};

        constexpr auto operator==(const Pairable auto &other) const -> bool
        {
            if constexpr (ComparisonRule == HowToComparePair::DEFAULT) {
                return equal(first, other.first) && equal(second, other.second);
            } else if constexpr (ComparisonRule == HowToComparePair::BY_FIRST_VALUE) {
                return equal(first, other.first);
            } else {
                return equal(second, other.second);
            }
        }

        constexpr auto operator<=>(const Pairable auto &other) const
        {
            if constexpr (ComparisonRule == HowToComparePair::DEFAULT) {
                if (equal(first, other.first)) {
                    return second <=> other.second;
                }
                return first <=> other.first;
            } else if constexpr (ComparisonRule == HowToComparePair::BY_FIRST_VALUE) {
                return first <=> other.first;
            } else {
                return second <=> other.second;
            }
        }

        constexpr auto operator=(const Pairable auto &pair) -> Pair &
        {
            first  = pair.first;
            second = pair.second;
            return *this;
        }

        constexpr Pair() = default;

        constexpr explicit Pair(const T1 &first_) : first(first_), second()
        {}

        constexpr explicit Pair(T1 &&first_) noexcept : first(std::move(first_)), second()
        {}

        constexpr Pair(const T1 &first_, const T2 &second_) : first(first_), second(second_)
        {}

        constexpr Pair(T1 &&first_, T2 &&second_) noexcept
          : first(std::move(first_)), second(std::move(second_))
        {}

        constexpr explicit Pair(const Pairable auto &pair) : first(pair.first), second(pair.second)
        {}
    };

    template<HowToComparePair ComparisonRule = HowToComparePair::DEFAULT, typename T1, typename T2>
    constexpr auto make_pair(T1 &&first, T2 &&second) -> Pair<T1, T2, ComparisonRule>
    {
        return { std::forward<T1>(first), std::forward<T2>(second) };
    }

    template<HowToComparePair ComparisonRule = HowToComparePair::DEFAULT, typename T1, typename T2>
    constexpr auto make_pair(const T1 &first, const T2 &second) -> Pair<T1, T2, ComparisonRule>
    {
        return { first, second };
    }
}// namespace cerb

#endif /* LIBCERBERUS_PAIR_HPP */
