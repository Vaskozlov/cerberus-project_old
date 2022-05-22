#ifndef CERBERUS_CATCH_CATCH_HPP
#define CERBERUS_CATCH_CATCH_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/debug/location.hpp>
#include <cerberus/exception.hpp>
#include <cerberus/pair.hpp>
#include <fmt/format.h>

#define EXPECT_TRUE(value) debug::expectTrue(value, CERBLIB_LOCATION)
#define EXPECT_FALSE(value) debug::expectFalse(value, CERBLIB_LOCATION)

#define EXPECT_EQUAL(lhs, rhs) debug::expectEqual(lhs, rhs, CERBLIB_LOCATION)
#define EXPECT_NOT_EQUAL(lhs, rhs) debug::expectNotEqual(lhs, rhs, CERBLIB_LOCATION)

#define CANT_BE_REACHED EXPECT_TRUE(false)
#define MUST_BE_REACHED EXPECT_TRUE(true)

#define CERBERUS_TEST(value)                                                                       \
    static_assert(value);                                                                          \
    EXPECT_TRUE(value)

#define CERBERUS_TEST_FUNC(name) CERBLIB_DECL auto name()->bool

namespace cerb::debug
{
    CERBERUS_EXCEPTION(RuntimeError, std::exception);
    CERBERUS_EXCEPTION(CompileTimeError, std::exception);

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
        if (lhs != static_cast<T1>(rhs)) {
            if CERBLIB_COMPILE_TIME {
                throw CompileTimeError("Cerberus test failure, because object are not equal!");
            }

            failureNotEqual<T1>(lhs, static_cast<T1>(rhs), loc);
        }
    }

    template<typename T1, typename T2>
    constexpr auto expectNotEqual(T1 const &lhs, T2 const &rhs, Location const &loc) -> void
    {
        if (lhs == static_cast<T1>(rhs)) {
            if CERBLIB_COMPILE_TIME {
                throw CompileTimeError("Cerberus test failure, because object are not equal!");
            }

            failureEqual<T1>(lhs, static_cast<T1>(rhs), loc);
        }
    }
}// namespace cerb::debug

#endif /* CERBERUS_CATCH_CATCH_HPP */
