#ifndef LIBCERBERUS_CATCH_CATCH_HPP
#define LIBCERBERUS_CATCH_CATCH_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/debug/location.hpp>
#include <cerberus/exception.hpp>
#include <cerberus/pair.hpp>
#include <fmt/color.h>
#include <fmt/format.h>

#define EXPECT_TRUE(value) debug::expectTrue(value, CERBLIB_LOCATION)
#define EXPECT_FALSE(value) debug::expectFalse(value, CERBLIB_LOCATION)

#define CR_EXPECT_TRUE(value)                                                                      \
    static_assert(value);                                                                          \
    EXPECT_TRUE(value)
#define CR_EXPECT_FALSE(value) static_assert(not (value); EXPECT_FALSE(value)

namespace cerb::debug
{
    CERBERUS_EXCEPTION(RuntimeError);
    CERBERUS_EXCEPTION(CompileTimeError);

    using PairedNumbers = Pair<ssize_t, double>;

    inline auto failure(bool condition, Location const &loc = CERBLIB_LOCATION) -> void
    {
        fmt::print(fmt::fg(fmt::color::red), "Cerberus test failure with code: {}! ", condition);
        fmt::print("File: {}, getLine: {}\n", loc.getFilename(), loc.getLine());
        throw RuntimeError();
    }

    template<std::integral T>
    constexpr auto expectTrue(T condition, Location const &loc = CERBLIB_LOCATION) -> void
    {
        if (not static_cast<bool>(condition)) {
            if CERBLIB_COMPILE_TIME {
                throw CompileTimeError("Cerberus test failure!");
            } else {
                failure(condition, loc);
            }
        }
    }

    template<std::integral T>
    constexpr auto expectFalse(T condition, Location const &loc = CERBLIB_LOCATION) -> void
    {
        if (static_cast<bool>(condition)) {
            if CERBLIB_COMPILE_TIME {
                throw CompileTimeError("Cerberus test failure!");
            } else {
                failure(condition, loc);
            }
        }
    }
}// namespace cerb::debug

#endif /* LIBCERBERUS_CATCH_CATCH_HPP */
