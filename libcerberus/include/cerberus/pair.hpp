#ifndef LIBCERBERUS_PAIR_HPP
#define LIBCERBERUS_PAIR_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/type.hpp>

namespace cerb {
    enum struct PairComparisonRule
    {
        DEFAULT,
        BY_FIRST_VALUE,
        BY_SECOND_VALUE
    };

    template<
        typename T1, typename T2,
        PairComparisonRule Compare = PairComparisonRule::DEFAULT>
    struct CERBLIB_TRIVIAL Pair
    {
        T1 first{};
        T2 second{};

        constexpr auto operator==(const Pairable auto &other) const -> bool
        {
            if constexpr (Compare == PairComparisonRule::DEFAULT) {
                return first == other.first && second == other.second;
            } else if constexpr (Compare == PairComparisonRule::BY_FIRST_VALUE) {
                return first == other.first;
            } else {
                return second == other.second;
            }
        }

        constexpr auto operator<=>(const Pairable auto &other) const
        {
            if constexpr (Compare == PairComparisonRule::DEFAULT) {
                if (first == other.first) {
                    return second <=> other.second;
                }
                return first <=> other.first;
            } else if constexpr (Compare == PairComparisonRule::BY_FIRST_VALUE) {
                return first <=> other.first;
            } else {
                return second <=> other.second;
            }
        }

        constexpr auto operator=(const Pair &pair) -> Pair & = default;
        constexpr auto operator=(Pair &&pair) noexcept -> Pair & = default;

        constexpr auto operator=(const Pairable auto &pair) -> Pair &
        {
            if (this != &pair) {
                first  = pair.first;
                second = pair.second;
            }

            return *this;
        }

        constexpr auto operator=(Pairable auto &&pair) noexcept -> Pair &
        {
            if (this != &pair) {
                first  = std::move(pair.first);
                second = std::move(pair.second);
            }

            return *this;
        }

        constexpr Pair()  = default;
        constexpr ~Pair() = default;

        constexpr Pair(const T1 &first_) : first(first_), second()
        {}

        constexpr Pair(T1 &&first_) noexcept : first(std::move(first_)), second()
        {}

        constexpr Pair(const T1 &first_, const T2 &second_)
          : first(first_), second(second_)
        {}

        constexpr Pair(T1 &&first_, T2 &&second_) noexcept
          : first(std::move(first_)), second(std::move(second_))
        {}

        constexpr Pair(const Pair &pair) : first(pair.first), second(pair.second)
        {}

        constexpr Pair(Pair &&pair) noexcept
          : first(std::move(pair.first)), second(std::move(pair.second))
        {}

        constexpr explicit Pair(const Pairable auto &pair)
          : first(pair.first), second(pair.second)
        {}
    };

    template<
        PairComparisonRule Compare = PairComparisonRule::DEFAULT, typename T1,
        typename T2>
    constexpr auto make_pair(T1 &&first, T2 &&second) -> Pair<T1, T2, Compare>
    {
        return { std::move(first), std::move(second) };
    }

    template<
        PairComparisonRule Compare = PairComparisonRule::DEFAULT, typename T1,
        typename T2>
    constexpr auto make_pair(const T1 &first, const T2 &second) -> Pair<T1, T2, Compare>
    {
        return { first, second };
    }
}// namespace cerb

#endif /* LIBCERBERUS_PAIR_HPP */
