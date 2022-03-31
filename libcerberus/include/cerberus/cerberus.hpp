#ifndef LIBCERBERUS_CERBERUS_HPP
#define LIBCERBERUS_CERBERUS_HPP

#if !defined(__cplusplus) || (__cplusplus < 202002L)
#    error libcerberus requires at least C++20
#endif /* minimum requirements */

#include <algorithm>
#include <array>
#include <bit>
#include <cinttypes>
#include <cstddef>
#include <limits>
#include <string_view>
#include <type_traits>
#include <utility>
#include <version>

#ifndef bitsizeof
#    define bitsizeof(x) (sizeof(x) * 8UL)
#endif /* bitsizeof */

#ifndef CERBLIB_STRX
#    define CERBLIB_STRX(x) #    x
#endif /* CERBLIB_STRX */

#ifndef CERBLIB_STR
#    define CERBLIB_STR(x) CERBLIB_STRX(x)
#endif /* CERBLIB_STR */

#ifndef CERBLIB_JOIN_STR
#    define CERBLIB_JOIN_STR(x, y) CERBLIB_STR(x##y)
#endif /* CERBLIB_JOIN_STR */

#ifndef CERBLIB_AMD64
#    ifdef __x86_64__
#        define CERBLIB_AMD64 1
#    else
#        define CERBLIB_AMD64 0
#    endif
#endif /* CERBLIB_AMD64 */

#ifndef CERBLIB_64BIT
#    if INTPTR_MAX == INT32_MAX
#        define CERBLIB_64BIT 0
#    else
#        define CERBLIB_64BIT 1
#    endif
#endif /* CERBLIB_64BIT */

#ifndef CERBLIB_INLINE
#    if defined(_MSC_VER)
#        define CERBLIB_INLINE __forceinline
#    else
#        define CERBLIB_INLINE __attribute__((always_inline)) inline
#    endif
#endif /* CERBLIB_INLINE */

#ifndef CERBLIB_COMPILE_TIME
#    ifndef __cpp_if_consteval
#        define CERBLIB_COMPILE_TIME (std::is_constant_evaluated())
#    else
#        define CERBLIB_COMPILE_TIME consteval
#    endif
#endif /* CERBLIB_COMPILE_TIME */

#ifndef CERBLIB_RUNTIME
#    ifndef __cpp_if_consteval
#        define CERBLIB_RUNTIME (not std::is_constant_evaluated())
#    else
#        define CERBLIB_RUNTIME not consteval
#    endif
#endif /* CERBLIB_RUNTIME */

#ifndef CERBLIB_DECL
#    define CERBLIB_DECL [[nodiscard]] constexpr
#endif /* CERBLIB_DECL */

#ifndef CERBLIB_TRIVIAL
#    if defined(__clang__)// clang compiler might use stack to pass small structs
#        define CERBLIB_TRIVIAL [[clang::trivial_abi]]
#    else
#        define CERBLIB_TRIVIAL
#    endif
#endif /* CERBLIB_TRIVIAL */

#ifndef CERBLIB_DEPRECATED
#    define CERBLIB_DEPRECATED [[deprecated]]
#endif /* CERBLIB_DEPRECATED */

#ifndef CERBLIB_DEPRECATED_SUGGEST
#    define CERBLIB_DEPRECATED_SUGGEST(ALT) [[deprecated(ALT)]]
#endif /* CERBLIB_DEPRECATED_SUGGEST */

#ifndef CERBLIB_IGNORE
#    define CERBLIB_IGNORE(X)                                                                      \
        {                                                                                          \
            [[maybe_unused]] auto _ = (X);                                                         \
        }
#endif /* CERBLIB_IGNORE */

#ifndef CERBLIB_DO_PRAGMA
#    ifdef _MSC_VER
#        define CERBLIB_DO_PRAGMA(x) __pragma(#        x)
#    else
#        define CERBLIB_DO_PRAGMA(x) _Pragma(#        x)
#    endif
#endif /* CERBLIB_DO_PRAGMA */

#ifndef CERBLIB_PRAGMA
#    ifdef _MSC_VER
#        define CERBLIB_DO_PRAGMA(x) __pragma(#        x)
#        define CERBLIB_PRAGMA(compiler, x) CERBLIB_DO_PRAGMA(warning(x))
#    else
#        define CERBLIB_DO_PRAGMA(x) _Pragma(#        x)
#        define CERBLIB_PRAGMA(compiler, x) CERBLIB_DO_PRAGMA(compiler diagnostic x)
#    endif
#endif /* CERBLIB_PRAGMA */

#ifndef CERBLIB_UNROLL
#    if defined(__clang__)
#        define CERBLIB_UNROLL _Pragma(CERBLIB_STR(unroll))
#    elif (__GNUC__)
#        define CERBLIB_UNROLL _Pragma(CERBLIB_STR(GCC unroll))
#    else
#        define CERBLIB_UNROLL
#    endif
#endif /* CERBLIB_UNROLL */

#ifndef CERBLIB_UNROLL_N
#    if defined(__clang__)
#        define CERBLIB_UNROLL_N(N) _Pragma(CERBLIB_STR(unroll N))
#    elif (__GNUC__)
#        define CERBLIB_UNROLL_N(N) _Pragma(CERBLIB_STR(GCC unroll N))
#    else
#        define CERBLIB_UNROLL_N(N)
#    endif
#endif /* CERBLIB_UNROLL_N */

#ifndef CERBLIB_CLANG_DISABLE_WARNING
#    ifdef __clang__
#        define CERBLIB_CLANG_DISABLE_WARNING(clang_option)                                        \
            CERBLIB_PRAGMA(clang, push)                                                            \
            CERBLIB_PRAGMA(clang, ignored clang_option)
#    else
#        define CERBLIB_CLANG_DISABLE_WARNING(clang_option)
#    endif
#endif /* CERBLIB_CLANG_DISABLE_WARNING */

#ifndef CERBLIB_CLANG_ENABLE_WARNING
#    ifdef __clang__
#        define CERBLIB_CLANG_ENABLE_WARNING CERBLIB_PRAGMA(clang, pop)
#    else
#        define CERBLIB_CLANG_ENABLE_WARNING
#    endif
#endif /* CERBLIB_CLANG_ENABLE_WARNING */

#ifndef CERBLIB_UNUSED
#    define CERBLIB_UNUSED(x) [[maybe_unused]] x _
#endif /* CERBLIB_UNUSED */

namespace cerb
{
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    using f32 = float;
    using f64 = double;

    using uint = unsigned int;
    using ssize_t = intmax_t;

    constexpr auto operator"" _u8(unsigned long long value) -> u8
    {
        return static_cast<u8>(value);
    }

    constexpr auto operator"" _u16(unsigned long long value) -> u16
    {
        return static_cast<u16>(value);
    }

    constexpr auto operator"" _u32(unsigned long long value) -> u32
    {
        return static_cast<u32>(value);
    }

    constexpr auto operator"" _u64(unsigned long long value) -> u64
    {
        return static_cast<u64>(value);
    }

    template<typename F, typename... Ts>
    constexpr auto forEach(F &&function, Ts &&...args) -> size_t
    {
        (function(std::forward<Ts>(args)), ...);
        return sizeof...(args);
    }

    template<typename... Ts>
    CERBLIB_DECL auto sum(Ts... args) -> decltype(auto)
    {
        return (... + args);
    }

    template<typename... Ts>
    CERBLIB_DECL auto prod(Ts... args) -> decltype(auto)
    {
        return (... * args);
    }

    template<typename... Ts>
    CERBLIB_DECL auto logicalAnd(Ts... args) -> bool
    {
        return (... & args);
    }

    template<typename... Ts>
    CERBLIB_DECL auto logicalOr(Ts... args) -> bool
    {
        return (... | args);
    }

    template<typename T>
    constexpr auto cmov(bool condition, T const &on_true, T const &on_false) -> T
    {
        return condition ? on_true : on_false;
    }
}// namespace cerb

#endif /* LIBCERBERUS_CERBERUS_HPP */
