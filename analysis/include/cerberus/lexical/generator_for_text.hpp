#ifndef CERBERUS_GENERATOR_FOR_TEXT_HPP
#define CERBERUS_GENERATOR_FOR_TEXT_HPP

#include <cerberus/lexical/location.hpp>
#include <string>

namespace cerb::lex {
    template<CharacterLiteral CharT = char>
    class GeneratorForText
    {
        using str = std::basic_string<CharT>;
        using str_view = BasicStringView<CharT>;

        template<std::integral T>
        CERBLIB_DECL static auto cast(T value) -> CharT
        {
            return static_cast<CharT>(value);
        }

    public:
        CERBLIB_DECL auto getLine() const -> size_t
        {
            return location.getLine();
        }

        CERBLIB_DECL auto getCharacterInLine() const -> size_t
        {
            return location.getCharacterInLine();
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

        CERBLIB_DECL auto getCharAtCurrentOffset(isize offset_for_offset = 0) const -> CharT
        {
            return text_data[static_cast<size_t>(
                static_cast<isize>(getOffset()) + offset_for_offset)];
        }

        CERBLIB_DECL auto skipLayoutAndGiveChar() -> CharT
        {
            skipLayout();
            return newRawChar();
        }

        CERBLIB_DECL auto newRawChar() -> CharT
        {
            if (isEndOfInput(getCharAtCurrentOffset())) {
                return cast('\0');
            }

            clearTabsAndSpacesIfNeeded();
            callSuitableFunctionForNextChar();

            return getCharAtCurrentOffset(-1);
        }

        constexpr auto skipLayout() -> void
        {
            while (lex::isLayout(getCharAtCurrentOffset())) {
                switch (getCharAtCurrentOffset()) {
                case cast('\0'):
                    break;

                case cast('\n'):
                    newLine();
                    break;

                case cast('\t'):
                case cast(' '):
                    tabs_and_spaces.push_back(getCharAtCurrentOffset());
                    [[fallthrough]];

                default:
                    location.newChar();
                    break;
                }
            }
        }

        struct iterator
        {
            constexpr auto operator++() -> iterator &
            {
                current_char = generator.skipLayoutAndGiveChar();
                return *this;
            }

            constexpr auto operator++(int) -> iterator &
            {
                auto save = *this;
                current_char = generator.skipLayoutAndGiveChar();
                return save;
            }

            constexpr auto operator*() const -> CharT
            {
                return current_char;
            }

            constexpr auto operator==(CharT other) const -> bool
            {
                return current_char == other;
            }

            constexpr auto operator==(iterator const &other) const -> bool
            {
                return current_char == other.current_char;
            }

            constexpr auto getCharAtCurrentOffset(i32 offset_for_current_char) const -> CharT
            {
                return generator.getCharAtCurrentOffset(offset_for_current_char);
            }

            constexpr iterator() = default;

            constexpr explicit iterator(CharT chr) : current_char(chr)
            {}

            constexpr explicit iterator(GeneratorForText const &gen)
              : generator(gen), current_char(generator.skipLayoutAndGiveChar())
            {}

        private:
            GeneratorForText generator{};
            CharT current_char{};
        };

        constexpr auto begin() -> iterator
        {
            return iterator(*this);
        }

        constexpr auto begin() const -> iterator
        {
            return iterator(*this);
        }

        constexpr auto end() -> iterator
        {
            return iterator(cast('\0'));
        }

        constexpr auto end() const -> iterator
        {
            return iterator(cast('\0'));
        }

        constexpr GeneratorForText() = default;
        constexpr GeneratorForText(
            LocationInFile const &current_location, str_view const &file_data)
          : location(current_location), text_data(file_data)
        {
            extractNewLine();
        }

    private:
        constexpr auto callSuitableFunctionForNextChar() -> void
        {
            if (getCharAtCurrentOffset() == cast('\n')) {
                newLine();
            } else {
                location.newChar();
            }
        }

        constexpr auto extractNewLine() -> void
        {
            auto begin_of_line = text_data.begin() + getOffset();
            auto end_of_line = text_data.begin() + text_data.find(cast('\n'), getOffset());
            current_line = { begin_of_line, end_of_line };
        }

        constexpr auto clearTabsAndSpacesIfNeeded() -> void
        {
            if (needToClearTabsAndSpaces()) {
                tabs_and_spaces.clear();
            }
        }

        CERBLIB_DECL auto needToClearTabsAndSpaces() -> bool
        {
            return logicalAnd(
                tabs_and_spaces.size() > 0, getCharAtCurrentOffset(-1) != cast('\t'),
                getCharAtCurrentOffset(-1) != cast(' '));
        }

        constexpr auto newLine() -> void
        {
            location.newLine();
            clearTabsAndSpacesIfNeeded();
            extractNewLine();
        }

        LocationInFile location{};
        str tabs_and_spaces{};
        str_view text_data{};
        str_view current_line{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_GENERATOR_FOR_TEXT_HPP */
