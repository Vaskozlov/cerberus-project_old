#ifndef LIBCERBERUS_CATCH_CATCH_HPP
#define LIBCERBERUS_CATCH_CATCH_HPP

#include <cerberus/cerberus.hpp>
#include <fmt/color.h>
#include <fmt/format.h>
#include <iostream>
#include <random>
#include <string_view>

#define CERBLIB_LOCATION cerb::test::location(__FILE__, __LINE__)

namespace cerb::test {
    class constexpr_failure final : public std::exception
    {
        std::string_view message;

    public:
        explicit constexpr_failure(std::string_view t_message) : message(t_message)
        {}

        constexpr_failure(constexpr_failure &&) noexcept      = default;
        constexpr_failure(const constexpr_failure &) noexcept = default;

        ~constexpr_failure() override = default;

        auto operator=(constexpr_failure &&) noexcept -> constexpr_failure & = default;
        auto operator              =(const constexpr_failure &) noexcept
            -> constexpr_failure & = default;

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
        [[nodiscard]] auto line() const -> size_t
        {
            return m_line;
        }

        [[nodiscard]] auto filename() const -> std::string_view
        {
            return m_filename;
        }

        constexpr auto operator=(const location &) -> location & = default;
        constexpr auto operator=(location &&) noexcept -> location & = default;

        constexpr location()  = default;
        constexpr ~location() = default;

        constexpr location(const location &)     = default;
        constexpr location(location &&) noexcept = default;

        constexpr location(std::string_view filename, std::size_t line)
          : m_filename(filename), m_line(line)
        {}
    };

    template<typename... Ts>
    constexpr auto
        expect_true(auto &&function, const location &loc = CERBLIB_LOCATION, Ts &&...args)
            -> void
    {
        if (auto code = function(std::forward<Ts>(args)...); code != 1) {
            if (std::is_constant_evaluated()) {
                throw constexpr_failure("Cerberus test failure!");
            } else {
                fmt::print(
                    fmt::fg(fmt::color::red),
                    "Cerberus test failure with code: {}! ",
                    code);
                fmt::print("File: {}, line: {}\n", loc.filename(), loc.line());
                exit(1);
            }
        }
    }

    template<typename... Ts>
    constexpr auto expect_false(
        auto &&function,
        const location &loc = CERBLIB_LOCATION,
        Ts &&...args) -> void
    {
        if (auto code = function(std::forward<Ts>(args)...); code != 0) {
            if (std::is_constant_evaluated()) {
                throw constexpr_failure("Cerberus test failure!");
            } else {
                fmt::print(
                    fmt::fg(fmt::color::red),
                    "Cerberus test failure with code: {}! ",
                    code);
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
