#ifndef CERBERUS_CATCH_CATCH_HPP
#define CERBERUS_CATCH_CATCH_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/debug/location.hpp>
#include <cerberus/exception.hpp>
#include <cerberus/pair.hpp>
#include <fmt/format.h>
#include <vector>

#define ASSERT_TRUE(value) debug::expectTrue(value, CERBLIB_LOCATION)
#define ASSERT_FALSE(value) debug::expectFalse(value, CERBLIB_LOCATION)

#define ASSERT_EQUAL(lhs, rhs) debug::expectEqual(lhs, rhs, CERBLIB_LOCATION)
#define ASSERT_NOT_EQUAL(lhs, rhs) debug::expectNotEqual(lhs, rhs, CERBLIB_LOCATION)

#define CANT_BE_REACHED ASSERT_TRUE(false)
#define MUST_BE_REACHED ASSERT_TRUE(true)
#define ERROR_EXPECTED(x, error_type, error_message)                                               \
    try {                                                                                          \
        x;                                                                                         \
        CANT_BE_REACHED;                                                                           \
    } catch (error_type const &error) {                                                            \
        ASSERT_EQUAL(std::basic_string_view(error.what()), error_message);                         \
    }


#define CERBERUS_TEST(value)                                                                       \
    static_assert(value);                                                                          \
    ASSERT_TRUE(value)

#if CERBLIB_CONSTEXPR_STD_VECTOR_AVAILABLE
#    define CERBLIB_VECTOR_CONSTEXPR constexpr
#else
#    define CERBLIB_VECTOR_CONSTEXPR
#endif

#if CERBLIB_CONSTEXPR_STD_STRING_AVAILABLE
#    define CERBLIB_STRING_CONSTEXPR constexpr
#else
#    define CERBLIB_STRING_CONSTEXPR
#endif

#define CERBERUS_TEST_FUNC(name) CERBLIB_DECL auto name()->bool
#define CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(name) CERBLIB_STRING_CONSTEXPR auto name()->bool
#define CERBERUS_TEST_FUNC_WITH_CONSTEXPR_VECTOR(name) CERBLIB_VECTOR_CONSTEXPR auto name()->bool

#if CERBLIB_CONSTEXPR_STD_VECTOR_AVAILABLE && CERBLIB_CONSTEXPR_STD_STRING_AVAILABLE
#    define CERBERUS_TEST_FUNC_WITH_CONSTEXPR_VECTOR_AND_STRING(name) constexpr auto name()->bool
#else
#    define CERBERUS_TEST_FUNC_WITH_CONSTEXPR_VECTOR_AND_STRING(name) auto name()->bool
#endif

#if CERBLIB_CONSTEXPR_STD_STRING_AVAILABLE
#    define CERBERUS_TEST_STD_STRING(value) CERBERUS_TEST(value)
#else
#    define CERBERUS_TEST_STD_STRING(value) ASSERT_TRUE(value)
#endif

#if CERBLIB_CONSTEXPR_STD_VECTOR_AVAILABLE
#    define CERBERUS_TEST_FOR_CONSTEXPR_VECTOR(value) CERBERUS_TEST(value)
#else
#    define CERBERUS_TEST_FOR_CONSTEXPR_VECTOR(value) ASSERT_TRUE(value)
#endif

#if CERBLIB_CONSTEXPR_STD_VECTOR_AVAILABLE && CERBLIB_CONSTEXPR_STD_STRING_AVAILABLE
#    define CERBERUS_TEST_FOR_CONSTEXPR_STRING(value) CERBERUS_TEST(value)
#else
#    define CERBERUS_TEST_FOR_CONSTEXPR_STRING(value) ASSERT_TRUE(value)
#endif

namespace cerb::debug
{
    CERBERUS_EXCEPTION(RuntimeError, cerb::CerberusException);
    CERBERUS_EXCEPTION(CompileTimeError, cerb::CerberusException);

    template<typename T>
    concept Printable = std::is_constructible_v<fmt::formatter<T>>;

    inline auto failure(bool condition, Location const &loc) -> void
    {
        throw RuntimeError(fmt::format(
            "Cerberus test failure with code: {}! File: {}, getLine: {}", condition,
            loc.getFilename(), loc.getLine()));
    }

    template<typename T>
    inline auto failureEqual(T const &lhs, T const &rhs, Location const &loc) -> void
    {
        if constexpr (Printable<T>) {
            throw RuntimeError(fmt::format(
                "Cerberus test failure, because object are equal!\n\tFirst: {}\n\tSecond: {}\n\t"
                "File: {}, getLine: {}",
                lhs, rhs, loc.getFilename(), loc.getLine()));
        }
        failure(false, loc);
    }

    template<typename T>
    inline auto failureNotEqual(T const &lhs, T const &rhs, Location const &loc) -> void
    {
        if constexpr (Printable<T>) {
            throw RuntimeError(fmt::format(
                "Cerberus test failure, because object are not equal!\n\tFirst: {}\n\tSecond: "
                "{}\n\tFile: {}, getLine: {}",
                lhs, rhs, loc.getFilename(), loc.getLine()));
        }
        failure(false, loc);
    }

    template<std::integral T>
    constexpr auto expectTrue(T condition, Location const &loc) -> void
    {
        if (not static_cast<bool>(condition)) {
            if CERBLIB_COMPILE_TIME {
                throw CompileTimeError("Cerberus test failure!");
            }
            failure(condition, loc);
        }
    }

    template<std::integral T>
    constexpr auto expectFalse(T condition, Location const &loc) -> void
    {
        if (static_cast<bool>(condition)) {
            if CERBLIB_COMPILE_TIME {
                throw CompileTimeError("Cerberus test failure!");
            }

            failure(condition, loc);
        }
    }

    template<typename T1, typename T2>
    constexpr auto expectEqual(T1 const &lhs, T2 const &rhs, Location const &loc) -> void
    {
        if (safeNotEqual(lhs, static_cast<T1>(rhs))) {
            if CERBLIB_COMPILE_TIME {
                throw CompileTimeError("Cerberus test failure, because object are not equal!");
            }

            failureNotEqual<T1>(lhs, static_cast<T1>(rhs), loc);
        }
    }

    template<typename T1, typename T2>
    constexpr auto expectNotEqual(T1 const &lhs, T2 const &rhs, Location const &loc) -> void
    {
        if (safeEqual(lhs, static_cast<T1>(rhs))) {
            if CERBLIB_COMPILE_TIME {
                throw CompileTimeError("Cerberus test failure, because object are not equal!");
            }

            failureEqual<T1>(lhs, static_cast<T1>(rhs), loc);
        }
    }
}// namespace cerb::debug

#endif /* CERBERUS_CATCH_CATCH_HPP */
