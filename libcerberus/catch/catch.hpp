#ifndef LIBCERBERUS_CATCH_CATCH_HPP
#define LIBCERBERUS_CATCH_CATCH_HPP

#include <cerberus/cerberus.hpp>
#include <fmt/color.h>
#include <fmt/format.h>
#include <iostream>
#include <string_view>

#define CERBLIB_LOCATION cerb::test::location(__FILE__, __LINE__)

namespace cerb::test {
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

        constexpr auto operator=(const location &) -> location & = default;
        constexpr auto operator=(location &&) noexcept -> location & = default;

        constexpr location()  = default;
        constexpr ~location() = default;

        constexpr location(const location &)     = default;
        constexpr location(location &&) noexcept = default;

        constexpr location(std::string_view filename, size_t line)
          : m_filename(filename), m_line(line)
        {}
    };

    template<typename... Ts>
    auto
        expect_true(auto &&function, const location &loc = CERBLIB_LOCATION, Ts &&...args)
            -> void
    {
        if (auto code = function(std::forward<Ts...>(args)...);
            code != 1) {
            fmt::print(
                fmt::fg(fmt::color::red), "Cerberus test failure with code: {}! ", code);
            fmt::print("File: {}, line: {}\n", loc.filename(), loc.line());
            exit(1);
        }
    }

    template<typename... Ts>
    auto expect_false(
        auto &&function, const location &loc = CERBLIB_LOCATION, Ts &&...args) -> void
    {
        if (auto code = function(std::forward<Ts...>(args)...); code != 0) {
            fmt::print(
                fmt::fg(fmt::color::red), "Cerberus test failure with code: {}! ", code);
            fmt::print("File: {}, line: {}\n", loc.filename(), loc.line());
            exit(1);
        }
    }
}// namespace cerb::test

#endif /* LIBCERBERUS_CATCH_CATCH_HPP */
