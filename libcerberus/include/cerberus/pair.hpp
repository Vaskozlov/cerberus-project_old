#ifndef LIBCERBERUS_PAIR_HPP
#define LIBCERBERUS_PAIR_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/number.hpp>

namespace cerb
{
    enum struct PairComparison : u32
    {
        DEFAULT,
        BY_FIRST_VALUE,
        BY_SECOND_VALUE
    };

    template<typename T1, typename T2, PairComparison ComparisonRule = PairComparison::DEFAULT>
    struct CERBLIB_TRIVIAL Pair
    {
        template<Pairable PairT>
        CERBLIB_DECL auto operator==(PairT const &other) const -> bool
        {
            if constexpr (ComparisonRule == PairComparison::DEFAULT) {
                return safeEqual<T1>(first, other.first) && safeEqual<T2>(second, other.second);
            } else if constexpr (ComparisonRule == PairComparison::BY_FIRST_VALUE) {
                return safeEqual<T1>(first, other.first);
            } else {
                return safeEqual<T2>(second, other.second);
            }
        }

        CERBLIB_DECL auto operator==(auto const &other) const -> bool
        {
            static_assert(
                ComparisonRule == PairComparison::BY_FIRST_VALUE ||
                ComparisonRule == PairComparison::BY_SECOND_VALUE);

            if constexpr (ComparisonRule == PairComparison::BY_FIRST_VALUE) {
                return safeEqual<T1>(first, other);
            } else if constexpr (ComparisonRule == PairComparison::BY_SECOND_VALUE) {
                return safeEqual<T2>(second, other);
            }
        }

        template<Pairable PairT>
        CERBLIB_DECL auto operator<=>(PairT const &other) const
        {
            if constexpr (ComparisonRule == PairComparison::DEFAULT) {
                if (safeEqual<T1>(first, other.first)) {
                    return second <=> other.second;
                }
                return first <=> other.first;
            } else if constexpr (ComparisonRule == PairComparison::BY_FIRST_VALUE) {
                return first <=> other.first;
            } else {
                return second <=> other.second;
            }
        }

        CERBLIB_DECL auto operator<=>(auto const &other) const
        {
            static_assert(
                ComparisonRule == PairComparison::BY_FIRST_VALUE ||
                ComparisonRule == PairComparison::BY_SECOND_VALUE);

            if constexpr (ComparisonRule == PairComparison::BY_FIRST_VALUE) {
                return first <=> other;
            } else if constexpr (ComparisonRule == PairComparison::BY_SECOND_VALUE) {
                return second <=> other;
            }
        }

        template<Pairable PairT>
        constexpr auto operator=(PairT const &pair) -> Pair &
        {
            first = pair.first;
            second = pair.second;
            return *this;
        }

        constexpr Pair() = default;

        constexpr explicit Pair(T1 const &first_value) : first(first_value), second()
        {}

        constexpr explicit Pair(T1 &&first_value) noexcept : first(std::move(first_value)), second()
        {}

        constexpr Pair(T1 const &first_value, T2 const &second_value)
          : first(first_value), second(second_value)
        {}

        constexpr Pair(T1 &&first_value, T2 &&second_value) noexcept
          : first(std::move(first_value)), second(std::move(second_value))
        {}

        template<Pairable PairT>
        constexpr explicit Pair(PairT const &pair) : first(pair.first), second(pair.second)
        {}

        T1 first{}; // NOLINT
        T2 second{};// NOLINT
    };

    template<PairComparison ComparisonRule = PairComparison::DEFAULT, Pairable T>
    constexpr auto makePair(T &&pair) -> decltype(auto)
    {
        return cerb::Pair<decltype(pair.first), decltype(pair.second), ComparisonRule>{
            std::move(pair.first), std::move(pair.second)
        };
    }

    template<PairComparison ComparisonRule = PairComparison::DEFAULT, Pairable T>
    constexpr auto makePair(T const &pair) -> decltype(auto)
    {
        return cerb::Pair<decltype(pair.first), decltype(pair.second), ComparisonRule>{
            pair.first, pair.second
        };
    }

    template<PairComparison ComparisonRule = PairComparison::DEFAULT, typename T1, typename T2>
    constexpr auto makePair(T1 &&first, T2 &&second) -> Pair<T1, T2, ComparisonRule>
    {
        return { std::forward<T1>(first), std::forward<T2>(second) };
    }

    template<PairComparison ComparisonRule = PairComparison::DEFAULT, typename T1, typename T2>
    constexpr auto makePair(T1 const &first, T2 const &second) -> Pair<T1, T2, ComparisonRule>
    {
        return { first, second };
    }
}// namespace cerb

#endif /* LIBCERBERUS_PAIR_HPP */
