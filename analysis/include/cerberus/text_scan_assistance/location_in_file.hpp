#ifndef CERBERUS_LOCATION_IN_FILE_HPP
#define CERBERUS_LOCATION_IN_FILE_HPP

#include <cerberus/string_view.hpp>

#define CERBLIB_LOCATION_IN_FILE_ACCESS(CharT)                                                     \
    using location_t = cerb::text::LocationInFile<CharT>;                                          \
    using location_t::getLine;                                                                     \
    using location_t::getOffset;                                                                   \
    using location_t::getColumn;                                                                   \
    using location_t::getFilename

namespace cerb::text
{
    template<CharacterLiteral CharT = char>
    struct LocationInFile
    {
        CERBLIB_DECL auto getLine() const -> size_t
        {
            return line;
        }

        CERBLIB_DECL auto getColumn() const -> size_t
        {
            return column;
        }

        CERBLIB_DECL auto getOffset() const -> size_t
        {
            return char_offset;
        }

        CERBLIB_DECL auto getFilename() const -> BasicStringView<CharT> const &
        {
            return name_of_file;
        }

        constexpr auto newChar() -> void
        {
            ++char_offset;
            ++column;
        }

        constexpr auto newLine() -> void
        {
            ++char_offset;
            ++line;
            column = 0;
        }

        LocationInFile() = default;

        constexpr explicit LocationInFile(
            BasicStringView<CharT> const &filename, size_t current_line = 1,
            size_t current_column = 1, size_t offset = 0)
          : name_of_file(filename), line(current_line), column(current_column), char_offset(offset)
        {}

    private:
        BasicStringView<CharT> name_of_file{};
        size_t line{};
        size_t column{};
        size_t char_offset{};
    };
}// namespace cerb::text

#endif /* CERBERUS_LOCATION_IN_FILE_HPP */
