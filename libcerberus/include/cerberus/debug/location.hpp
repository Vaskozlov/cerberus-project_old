#ifndef CERBERUS_LOCATION_HPP
#define CERBERUS_LOCATION_HPP

#include <cerberus/cerberus.hpp>
#include <string_view>

#define CERBLIB_LOCATION cerb::Location(__FILE__, __LINE__)

namespace cerb
{
    struct Location
    {
        CERBLIB_DECL auto getLine() const -> size_t
        {
            return line;
        }

        CERBLIB_DECL auto getFilename() const -> std::string_view const &
        {
            return filename;
        }

        Location() = default;
        constexpr Location(
            std::string_view const &filename_of_location, std::size_t line_of_location)
          : filename(filename_of_location), line(line_of_location)
        {}

    private:
        std::string_view filename{};
        size_t line{};
    };
}// namespace cerb

#endif /* CERBERUS_LOCATION_HPP */
