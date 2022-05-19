#ifndef CERBERUS_CATCH_CATCH_HPP
#define CERBERUS_CATCH_CATCH_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/debug/location.hpp>
#include <cerberus/exception.hpp>
#include <cerberus/pair.hpp>
#include <fmt/format.h>

#define EXPECT_TRUE(value) debug::expectTrue(value, CERBLIB_LOCATION)
#define EXPECT_FALSE(value) debug::expectFalse(value, CERBLIB_LOCATION)

#define CANT_BE_REACHED EXPECT_TRUE(false)
#define MUST_BE_REACHED EXPECT_TRUE(true)

#define CERBERUS_TEST(value)                                                                       \
    static_assert(value);                                                                          \
    EXPECT_TRUE(value)

namespace cerb::debug
{
    CERBERUS_EXCEPTION(RuntimeError, std::exception);
    CERBERUS_EXCEPTION(CompileTimeError, std::exception);

    inline auto failure(bool condition, Location const &loc = CERBLIB_LOCATION) -> void
    {
        throw RuntimeError(fmt::format(
            "Cerberus test failure with code: {}! File: {}, getLine: {}\n", condition,
            loc.getFilename(), loc.getLine()));
    }

    template<std::integral T>
    constexpr auto expectTrue(T condition, Location const &loc = CERBLIB_LOCATION) -> void
    {
        if (not static_cast<bool>(condition)) {
            if CERBLIB_COMPILE_TIME {
                throw CompileTimeError("Cerberus test failure!");
            }

            failure(condition, loc);
        }
    }

    template<std::integral T>
    constexpr auto expectFalse(T condition, Location const &loc = CERBLIB_LOCATION) -> void
    {
        if (static_cast<bool>(condition)) {
            if CERBLIB_COMPILE_TIME {
                throw CompileTimeError("Cerberus test failure!");
            }

            failure(condition, loc);
        }
    }
}// namespace cerb::debug

#endif /* CERBERUS_CATCH_CATCH_HPP */
