#ifndef LIBCERBERUS_CATCH_CATCH_HPP
#define LIBCERBERUS_CATCH_CATCH_HPP

#include "cerberus.hpp"
#include "pair.hpp"
#include <fmt/color.h>
#include <fmt/format.h>
#include <iostream>
#include <random>
#include <string_view>

#define CERBLIB_LOCATION cerb::test::Location(__FILE__, __LINE__)
#define EXPECT_TRUE(value) expectTrue(value, CERBLIB_LOCATION)
#define EXPECT_FALSE(value) expectFalse(value, CERBLIB_LOCATION)

namespace cerb::test {
    class ConstexprFailure : public std::exception
    {
        std::string_view message;

    public:
        ConstexprFailure() = default;

        explicit ConstexprFailure(std::string_view t_message) : message(t_message)
        {}

        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            return message.data();
        }
    };

    class Location
    {
        std::string_view filename{};
        size_t line{};

    public:
        CERBLIB_DECL auto getLine() const -> size_t
        {
            return line;
        }

        CERBLIB_DECL auto getFilename() const -> std::string_view
        {
            return filename;
        }

        constexpr Location() = default;
        constexpr Location(std::string_view filename_of_location, std::size_t line_of_location)
          : filename(filename_of_location), line(line_of_location)
        {}
    };

    using PairedNumbers = Pair<isize, double>;

    constexpr auto expectTrue(bool condition, const Location &loc = CERBLIB_LOCATION) -> void
    {
        if (!condition) {
            if (std::is_constant_evaluated()) {
                throw ConstexprFailure("Cerberus test failure!");
            } else {
                fmt::print(
                    fmt::fg(fmt::color::red), "Cerberus test failure with code: {}! ", condition);
                fmt::print("File: {}, getLine: {}\n", loc.getFilename(), loc.getLine());
                exit(1);
            }
        }
    }

    constexpr auto expectFalse(bool condition, const Location &loc = CERBLIB_LOCATION) -> void
    {
        if (condition) {
            if (std::is_constant_evaluated()) {
                throw ConstexprFailure("Cerberus test failure!");
            } else {
                fmt::print(
                    fmt::fg(fmt::color::red), "Cerberus test failure with code: {}! ", condition);
                fmt::print("File: {}, getLine: {}\n", loc.getFilename(), loc.getLine());
                exit(1);
            }
        }
    }

    template<std::integral T>
    auto createRandomArrayOfInts(size_t size) -> std::unique_ptr<T>
    {
        static std::random_device random_device;
        static std::mt19937 engine(random_device());
        std::unique_ptr<T> data =
            std::unique_ptr<T>(static_cast<T *>(::operator new(size * sizeof(T))));

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
