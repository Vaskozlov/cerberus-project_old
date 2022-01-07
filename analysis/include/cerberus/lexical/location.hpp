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
    struct TextGenerator
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

        CERBLIB_DECL auto getFilename() const -> string_view const &
        {
            return location.getFilename();
        }

        CERBLIB_DECL auto getLocation() const -> LocationInFile const &
        {
            return location;
        }

        CERBLIB_DECL auto getTabsAndSpaces() const -> std::basic_string<CharT> const &
        {
            return tabs_and_spaces;
        }

        CERBLIB_DECL auto getCurrentLine() const -> BasicStringView<CharT> const &
        {
            return current_line;
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
                newLine();
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
                case '\0':
                    break;
                case '\n':
                    newLine();
                    break;
                case '\t':
                case ' ':
                    tabs_and_spaces.push_back(data[offset]);
                    [[fallthrough]];
                default:
                    location.newChar();
                    break;
                }
                offset = getOffset();
            }
        }

        struct iterator
        {
            constexpr auto operator++() -> iterator &
            {
                current_char = generator->skipLayoutAndGiveNewChar();
                return *this;
            }

            constexpr auto operator==(iterator const &other) const -> bool
            {
                return current_char == other.current_char;
            }

            constexpr auto operator*() const -> CharT
            {
                return current_char;
            }

            constexpr auto nextChar() const -> CharT
            {
                return generator->nextYieldedChar();
            }

            constexpr iterator() = default;
            constexpr explicit iterator(CharT chr) : current_char(chr)
            {}
            constexpr explicit iterator(TextGenerator *gen)
              : generator(gen), current_char(generator->skipLayoutAndGiveNewChar())
            {}

        private:
            TextGenerator *generator{};
            CharT current_char{};
        };

        constexpr auto begin() -> iterator
        {
            return iterator(this);
        }

        constexpr auto end() -> iterator
        {
            return iterator('\0');
        }

    private:
        constexpr auto setCurrentLine() -> void
        {
            auto begin_of_the_line = data.begin() + location.getOffset();
            auto length = ptrdiff(data.end(), begin_of_the_line);

            current_line = { begin_of_the_line,
                             find(begin_of_the_line, '\n', static_cast<usize>(length)) };
        }

        constexpr auto newLine() -> void
        {
            location.newLine();
            setCurrentLine();
        }

        constexpr auto clearTabsAndSpacesIfNeeded() -> void
        {
            auto offset = getOffset();

            if (logicalAnd(
                    tabs_and_spaces.size() > 0, data[offset - 1] != '\t',
                    data[offset - 1] != ' ')) {
                tabs_and_spaces.clear();
            }
        }

    public:
        constexpr TextGenerator() = default;
        constexpr TextGenerator(
            const LocationInFile &current_location, BasicStringView<CharT> content)
          : location(current_location), data(content)
        {
            setCurrentLine();
        }

    private:
        LocationInFile location{};
        BasicStringView<CharT> data{};
        std::basic_string<CharT> tabs_and_spaces{};
        BasicStringView<CharT> current_line{};
    };
}// namespace cerb

#endif /* CERBERUS_LOCATION_HPP */
