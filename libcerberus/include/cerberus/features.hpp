#ifndef CERBERUS_FEATURES_HPP
#define CERBERUS_FEATURES_HPP

#include <algorithm>
#include <bit>
#include <ranges>

#if defined(__clang__)
#    include <experimental/coroutine>
#elif defined(__GNUC__) || defined(_MSC_VER)
#    include <coroutine>
#endif

namespace cerb
{
#if defined(__clang__)
    using suspend_never = std::experimental::suspend_never;
    using suspend_never = std::experimental::suspend_never;

    template<typename T>
    using coroutine_handle = std::experimental::coroutine_handle<T>;

    template<typename To, typename From>
    CERBLIB_DECL auto bit_cast(const From &from) noexcept -> To
    {
#    ifdef _LIBCPP_VERSION
        return __builtin_bit_cast(To, from);
#    else
        return std::bit_cast<To>(from);
#    endif
    }

    namespace ranges
    {
        template<Iterable T>
        constexpr auto fill(T &object, GetValueType<T> const &value) -> void
        {
            std::fill(std::begin(object), std::end(object), value);
        }

        template<Iterable T>
        constexpr auto equal(T const &lhs, T const &rhs) -> bool
        {
            return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs));
        }

        template<Iterable T, typename F>
        constexpr auto all_of(T &object, F &&function) -> bool
        {
            return std::all_of(std::begin(object), std::end(object), std::forward<F>(function));
        }

        template<Iterable T, typename F>
        constexpr auto for_each(T &object, F &&function) -> void
        {
            std::for_each(std::begin(object), std::end(object), std::forward<F>(function));
        }

        template<Iterable T, typename U>
        constexpr auto find(T &object, U const &value) -> decltype(auto)
        {
            return std::find(object.begin(), object.end(), value);
        }
    }// namespace ranges

#elif defined(__GNUC__) || defined(_MSC_VER)

    using suspend_never = std::suspend_never;
    using suspend_never = std::suspend_never;

    template<typename T>
    using coroutine_handle = std::coroutine_handle<T>;

    template<typename To, typename From>
    CERBLIB_DECL auto bit_cast(const From &from) noexcept -> To
    {
        return std::bit_cast<To>(from);
    }

    namespace ranges
    {
        template<Iterable T>
        constexpr auto fill(T &object, GetValueType<T> const &value) -> void
        {
            std::ranges::fill(object, value);
        }

        template<Iterable T>
        constexpr auto equal(T const &lhs, T const &rhs) -> bool
        {
            return std::ranges::equal(lhs, rhs);
        }

        template<Iterable T, typename F>
        constexpr auto all_of(T &object, F &&function) -> bool
        {
            return std::ranges::all_of(object, std::forward<F>(function));
        }

        template<Iterable T, typename F>
        constexpr auto for_each(T &object, F &&function) -> void
        {
            std::ranges::for_each(object, std::forward<F>(function));
        }

        template<Iterable T, typename U>
        constexpr auto find(T &object, U const &value) -> decltype(auto)
        {
            return std::ranges::find(object, value);
        }
    }// namespace ranges

#endif
}// namespace cerb

#endif /* CERBERUS_FEATURES_HPP */
