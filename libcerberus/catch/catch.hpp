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

    auto test(auto &&function, const location &loc = CERBLIB_LOCATION, auto &&...args)
    {
        if (auto code = function(args...); code != 0) {
            fmt::print(fmt::fg(fmt::color::red), "Cerberus test failure! ");
            fmt::print("File: {}, line: {}\n", loc.filename(), loc.line());
            exit(code);
        }
    }

    template<typename T>
    auto random_array(size_t size) -> std::unique_ptr<T>
    {
        static std::random_device random_device;
        static std::mt19937 engine(random_device());
        static std::uniform_int_distribution<T> distribution(
            std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

        std::unique_ptr<T> data =
            std::unique_ptr<T>(static_cast<T *>(::operator new(size * sizeof(T))));
        for (size_t i = 0; i < size; ++i) {
            data.get()[i] = distribution(engine);
        }

        return std::move(data);
    }
}// namespace cerb::test

#endif /* LIBCERBERUS_CATCH_CATCH_HPP */
