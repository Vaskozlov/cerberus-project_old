#ifndef CERBERUS_FILE_LOCATION_HPP
#define CERBERUS_FILE_LOCATION_HPP

#include <cerberus/lexical/char.hpp>
#include <cerberus/string_view.hpp>

namespace cerb
{
    struct LocationInFile
    {
        CERBLIB_DECL auto getLine() const -> size_t
        {
            return line;
        }

        CERBLIB_DECL auto getCharacterInLine() const -> size_t
        {
            return character;
        }

        CERBLIB_DECL auto getOffset() const -> size_t
        {
            return offset;
        }

        CERBLIB_DECL auto getFilename() const -> cerb::string_view const &
        {
            return filename;
        }

        constexpr auto newChar() -> void
        {
            ++character;
            ++offset;
        }

        constexpr auto newLine() -> void
        {
            character = 0;
            ++line;
            ++offset;
        }

        constexpr LocationInFile() = default;
        constexpr explicit LocationInFile(
            cerb::string_view name_of_file, size_t current_line = 1, size_t current_character = 1,
            size_t chars_from_the_beginning = 0)
          : filename(name_of_file), line(current_line), character(current_character),
            offset(chars_from_the_beginning)
        {}

    private:
        cerb::string_view filename{};
        size_t line{};
        size_t character{};
        size_t offset{};
    };
}// namespace cerb

#endif /* CERBERUS_FILE_LOCATION_HPP */