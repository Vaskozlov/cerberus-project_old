#ifndef CERBERUS_LOCATION_HPP
#define CERBERUS_LOCATION_HPP

#include <cerberus/lexical/char.hpp>
#include <cerberus/string_view.hpp>

namespace cerb {
    struct LocationInFile
    {
        CERBLIB_DECL auto getLine() const -> size_t
        {
            return line;
        }

        CERBLIB_DECL auto getCharacter() const -> size_t
        {
            return character;
        }

        CERBLIB_DECL auto getOffset() const -> size_t
        {
            return offset;
        }

        CERBLIB_DECL auto getFilename() const -> cerb::string_view
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
            cerb::string_view name_of_file, size_t current_line = 0, size_t current_character = 0,
            size_t chars_from_the_beginning = 0)
          : line(current_line), character(current_character), offset(chars_from_the_beginning),
            filename(name_of_file)
        {}

    private:
        size_t line{};
        size_t character{};
        size_t offset{};
        cerb::string_view filename{};
    };

    template<CharacterLiteral CharT = char>
    struct TxtManager
    {
        CERBLIB_DECL auto getLine() const -> size_t
        {
            return location.getLine();
        }

        CERBLIB_DECL auto getCharacter() const -> size_t
        {
            return location.getCharacter();
        }

        CERBLIB_DECL auto getOffset() const -> size_t
        {
            return location.getOffset();
        }

        CERBLIB_DECL auto getFilename() const -> cerb::string_view
        {
            return location.getFilename();
        }

        CERBLIB_DECL auto getLocation() const -> LocationInFile
        {
            return location;
        }

        CERBLIB_DECL auto getTabsAndSpaces() const -> const std::basic_string<CharT> &
        {
            return tabs_and_spaces;
        }

        CERBLIB_DECL auto skipLayoutAndGiveNewChar() -> CharT
        {
            skipLayoutChars();
            return newChar();
        }

        CERBLIB_DECL auto newChar() -> CharT
        {
            auto offset = getOffset();

            if (lex::isEndOfInput(data[offset])) {
                return static_cast<CharT>(0);
            }

            clearTabsAndSpacesIfNeeded();

            if (data[offset] == '\n') {
                location.newLine();
            } else {
                location.newChar();
            }

            return data[offset];
        }

        CERBLIB_DECL auto nextYieldedChar() const -> CharT
        {
            auto offset = getOffset();
            return data[offset];
        }

        constexpr auto skipLayoutChars() -> void
        {
            auto offset = getOffset();

            while (lex::isLayout(data[offset])) {
                switch (data[offset]) {
                case '\n':
                    location.newLine();
                    break;
                case ' ':
                case '\t':
                    tabs_and_spaces.push_back(data[offset]);
                    [[fallthrough]];
                default:
                    location.newChar();
                    break;
                }
                offset = getOffset();
            }
        }

    private:
        constexpr auto clearTabsAndSpacesIfNeeded() -> void
        {
            auto offset = getOffset();

            if (tabs_and_spaces.size() > 0 &&
                (data[offset - 1] != '\t' && data[offset - 1] != ' ')) {
                tabs_and_spaces.clear();
            }
        }

    public:
        constexpr TxtManager() = default;
        constexpr TxtManager(
            const LocationInFile &current_location, cerb::BasicStringView<CharT> content)
          : location(current_location), data(content)
        {}

    private:
        LocationInFile location{};
        cerb::BasicStringView<CharT> data{};
        std::basic_string<CharT> tabs_and_spaces{};
    };
}// namespace cerb

#endif /* CERBERUS_LOCATION_HPP */
