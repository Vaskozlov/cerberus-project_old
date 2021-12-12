#ifndef LIBCERBERUS_CATCH_CATCH_HPP
#define LIBCERBERUS_CATCH_CATCH_HPP

#include <cerberus/cerberus.hpp>
#include <fmt/color.h>
#include <fmt/format.h>
#include <iostream>
#include <random>
#include <string_view>

#define CERBLIB_LOCATION cerb::test::location(__FILE__, __LINE__)
#define EXPECT_TRUE(value) expectTrue(value, CERBLIB_LOCATION)
#define EXPECT_FALSE(value) expectFalse(value, CERBLIB_LOCATION)

namespace cerb::test {
    class constexpr_failure final : public std::exception
    {
        std::string_view message;

    public:
        constexpr_failure() = default;

        explicit constexpr_failure(std::string_view t_message) : message(t_message)
        {}

        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            return message.data();
        }
    };

    class location
    {
        std::string_view m_filename{};
        size_t m_line{};

    public:
        CERBLIB_DECL auto line() const -> size_t
        {
            return m_line;
        }

        CERBLIB_DECL auto filename() const -> std::string_view
        {
            return m_filename;
        }

        constexpr location() = default;
        constexpr location(std::string_view filename, std::size_t line)
          : m_filename(filename), m_line(line)
        {}
    };

    struct ComplexValue
    {
        i64 m_int;
        double m_float;

        CERBLIB_CLANG_DISABLE_WARNING("-Wfloat-equal")
        constexpr auto operator<=>(const ComplexValue &) const = default;
        CERBLIB_CLANG_ENABLE_WARNING

        constexpr ComplexValue() = default;
        constexpr ComplexValue(i64 int_, double float_) : m_int(int_), m_float(float_)
        {}
    };

    constexpr auto expectTrue(bool condition, const location &loc = CERBLIB_LOCATION) -> void
    {
        if (!condition) {
            if (std::is_constant_evaluated()) {
                throw constexpr_failure("Cerberus test failure!");
            } else {
                fmt::print(
                    fmt::fg(fmt::color::red), "Cerberus test failure with code: {}! ", condition);
                fmt::print("File: {}, line: {}\n", loc.filename(), loc.line());
                exit(1);
            }
        }
    }

    constexpr auto expectFalse(bool condition, const location &loc = CERBLIB_LOCATION) -> void
    {
        if (condition) {
            if (std::is_constant_evaluated()) {
                throw constexpr_failure("Cerberus test failure!");
            } else {
                fmt::print(
                    fmt::fg(fmt::color::red), "Cerberus test failure with code: {}! ", condition);
                fmt::print("File: {}, line: {}\n", loc.filename(), loc.line());
                exit(1);
            }
        }
    }

    template<typename T>
    auto random_array(size_t size) -> std::unique_ptr<T>
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
