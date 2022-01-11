#ifndef LIBCERBERUS_CERBERUS_HPP
#define LIBCERBERUS_CERBERUS_HPP

#if !defined(__cplusplus) || (__cplusplus < 202002L)
#    error libcerberus requires at least C++20
#endif /* minimum requirements */

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

#ifndef CERBLIB_INLINE
#    if defined(_MSC_VER)
#        define CERBLIB_INLINE __forceinline
#    else
#        define CERBLIB_INLINE __attribute__((always_inline)) inline
#    endif
#endif /* CERBLIB_INLINE */

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
#    define CERBLIB_IGNORE [[maybe_unused]] auto _ =
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

#include <algorithm>
#include <array>
#include <cinttypes>
#include <cstddef>
#include <limits>
#include <string_view>
#include <type_traits>
#include <utility>

namespace cerb {
    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    using f32 = float;
    using f64 = double;

    using isize = intmax_t;
    using usize = uintmax_t;

    using size_t = std::size_t;

#if defined(__WINDOWS__) || defined(__WIN32__)
    constexpr auto endl = "\n\r";
#else
    constexpr auto endl = '\n';
#endif

    /**
     * Just an empty type.
     */
    class EmptyType
    {
        CERBLIB_CLANG_DISABLE_WARNING("-Wunused-private-field")
        u8 empty{};
        CERBLIB_CLANG_ENABLE_WARNING

    public:
        constexpr EmptyType() noexcept = default;

        constexpr auto operator()() const -> EmptyType
        {
            return {};
        }
    };

    constexpr EmptyType Empty{};

    template<typename F, typename... Ts>
    constexpr auto call(F &&function, Ts &&...args) -> decltype(auto)
    {
        if constexpr (std::is_same_v<F, EmptyType>) {
            return;
        } else {
            return function(std::forward<Ts...>(args)...);
        }
    }

    template<typename T>
    CERBLIB_DECL auto getLimit(const T & /*unused*/) -> std::numeric_limits<T>
    {
        return std::numeric_limits<T>();
    }

    template<auto Begin, auto End, auto Inc>
    constexpr auto constexprFor(auto &&function) -> void
    {
        if constexpr (Begin < End) {
            function(std::integral_constant<decltype(Begin), Begin>());
            constexprFor<Begin + Inc, End, Inc>(function);
        }
    }

    /**
     * iterates through parameter pack
     * @tparam Ts
     * @param function which will be called for each argument
     * @param args arguments to iterate throw
     * @return
     */
    template<typename F, typename... Ts>
    CERBLIB_DECL auto forEach(F &&function, Ts &&...args)
    {
        [[maybe_unused]] const auto iterator = { ([&function]<typename T>(T &&value) {
            call(std::forward<F>(function), std::forward<T>(value));
            return 0;
        })(std::forward<Ts>(args))... };
    }

    template<typename... Ts>
    CERBLIB_DECL auto logicalAnd(bool first, Ts... other) -> bool
    {
        forEach([&first](bool value) { first &= value; }, other...);
        return first;
    }

    template<typename... Ts>
    CERBLIB_DECL auto logicalOr(bool first, Ts... other) -> bool
    {
        forEach([&first](bool value) { first |= value; }, other...);
        return first;
    }

    template<typename T>
    constexpr auto cmov(bool condition, const T &on_true, const T &on_false) -> T
    {
        return condition ? on_true : on_false;
    }
}// namespace cerb

#endif /* LIBCERBERUS_CERBERUS_HPP */
