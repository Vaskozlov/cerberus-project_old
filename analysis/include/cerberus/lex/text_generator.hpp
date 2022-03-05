#ifndef CERBERUS_TEXT_GENERATOR_HPP
#define CERBERUS_TEXT_GENERATOR_HPP

#include <cerberus/lex/char.hpp>
#include <cerberus/lex/location_in_file.hpp>
#include <string>

namespace cerb::lex
{
    template<CharacterLiteral CharT, CharacterLiteral FileNameT = char>
    struct TextGenerator
    {
        CERBLIB_DECL auto location() const -> LocationInFile<FileNameT> const &
        {
            return location_in_file;
        }

        CERBLIB_DECL auto tabsAndSpaces() const -> std::basic_string<CharT> const &
        {
            return tabs_and_spaces;
        }

        CERBLIB_DECL auto text() const -> BasicStringView<CharT> const &
        {
            return text_of_file;
        }

        CERBLIB_DECL auto currentLine() const -> BasicStringView<CharT> const &
        {
            return current_line;
        }

        constexpr TextGenerator() = default;
        constexpr TextGenerator(
            BasicStringView<CharT> const &file_content,
            BasicStringView<FileNameT> const &name_of_file)
          : location_in_file(name_of_file), text_of_file(file_content)
        {
            updateCurrentLine();
        }

    private:
        constexpr auto updateCurrentLine() -> void
        {
            auto offset = location_in_file.offset();
            auto text_begin = text_of_file.begin();
            auto begin_of_line = text_begin + offset;
            size_t line_size = text_of_file.find(CharEnum<CharT>::NewLine, offset) - offset;

            current_line = { begin_of_line, line_size };
        }

        LocationInFile<FileNameT> location_in_file{};
        std::basic_string<CharT> tabs_and_spaces{};
        BasicStringView<CharT> text_of_file{};
        BasicStringView<CharT> current_line{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_TEXT_GENERATOR_HPP */
