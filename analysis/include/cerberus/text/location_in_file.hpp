#ifndef CERBERUS_LOCATION_IN_FILE_HPP
#define CERBERUS_LOCATION_IN_FILE_HPP

#include <cerberus/string_view.hpp>

#define CERBLIB_LOCATION_IN_FILE_ACCESS(CharT)                                                     \
    using location_t = cerb::text::LocationInFile<CharT>;                                          \
    using location_t::line;                                                                        \
    using location_t::charOffset;                                                                  \
    using location_t::charPosition;                                                                \
    using location_t::filename

namespace cerb::text
{
    template<CharacterLiteral CharT = char>
    struct LocationInFile
    {
        CERBLIB_DECL auto line() const -> size_t
        {
            return line_number;
        }

        CERBLIB_DECL auto charPosition() const -> size_t
        {
            return char_number;
        }

        CERBLIB_DECL auto charOffset() const -> size_t
        {
            return char_offset;
        }

        CERBLIB_DECL auto filename() const -> BasicStringView<CharT> const &
        {
            return name_of_file;
        }

        constexpr auto resetOffset() -> void
        {
            char_offset = 0;
        }

        constexpr auto newChar() -> void
        {
            ++char_offset;
            ++char_number;
        }

        constexpr auto newLine() -> void
        {
            ++char_offset;
            ++line_number;
            char_number = 0;
        }

        LocationInFile() = default;

        constexpr explicit LocationInFile(
            BasicStringView<CharT> const &filename, size_t current_line = 1,
            size_t current_char = 1, size_t offset_from_the_begging = 0)
          : name_of_file(filename), line_number(current_line), char_number(current_char),
            char_offset(offset_from_the_begging)
        {}

    private:
        BasicStringView<CharT> name_of_file{};
        size_t line_number{};
        size_t char_number{};
        size_t char_offset{};
    };
}// namespace cerb::text

#endif /* CERBERUS_LOCATION_IN_FILE_HPP */
