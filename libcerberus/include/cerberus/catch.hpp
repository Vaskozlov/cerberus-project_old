#ifndef LIBCERBERUS_CATCH_CATCH_HPP
#define LIBCERBERUS_CATCH_CATCH_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/exception.hpp>
#include <cerberus/location.hpp>
#include <cerberus/pair.hpp>
#include <fmt/color.h>
#include <fmt/format.h>
#include <iostream>
#include <random>

#define EXPECT_TRUE(value) expectTrue(value, CERBLIB_LOCATION)
#define EXPECT_FALSE(value) expectFalse(value, CERBLIB_LOCATION)

namespace cerb::test
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

    constexpr auto expectTrue(bool condition, Location const &loc = CERBLIB_LOCATION) -> void
    {
        if (not condition) {
            if (std::is_constant_evaluated()) {
                throw CompileTimeError("Cerberus test failure!");
            } else {
                failure(condition, loc);
            }
        }
    }

    constexpr auto expectFalse(bool condition, Location const &loc = CERBLIB_LOCATION) -> void
    {
        if (condition) {
            if (std::is_constant_evaluated()) {
                throw CompileTimeError("Cerberus test failure!");
            } else {
                failure(condition, loc);
            }
        }
    }

    template<std::integral T>
    auto createRandomArrayOfInts(size_t size) -> std::unique_ptr<T[]>
    {
        static std::random_device random_device;
        static std::mt19937 engine(random_device());
        std::unique_ptr<T[]> data = std::make_unique<T[]>(size);

        if constexpr (std::is_unsigned_v<T>) {
            static std::uniform_int_distribution<size_t> distribution(
                std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

            for (size_t i = 0; i < size; ++i) {
                data.get()[i] = static_cast<T>(distribution(engine));
            }
        } else {
            static std::uniform_int_distribution<int64_t> distribution(
                std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

            for (size_t i = 0; i < size; ++i) {
                data.get()[i] = static_cast<T>(distribution(engine));
            }
        }

        return data;
    }
}// namespace cerb::test

#endif /* LIBCERBERUS_CATCH_CATCH_HPP */
