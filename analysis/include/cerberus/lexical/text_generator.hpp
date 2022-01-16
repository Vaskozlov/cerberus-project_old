#ifndef CERBERUS_TEXT_GENERATOR_HPP
#define CERBERUS_TEXT_GENERATOR_HPP

#include <cerberus/lexical/location.hpp>
#include <cerberus/reference_wrapper.hpp>
#include <string>

namespace cerb::lex
{
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
        struct iterator;
        struct referenced_iterator;

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

        CERBLIB_DECL auto getCurrentStateString() const -> str_view
        {
            return { text_data.begin() + getOffset(), text_data.end() };
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

        CERBLIB_DECL auto begin() -> referenced_iterator
        {
            return referenced_iterator(ReferenceWrapper(*this));
        }

        CERBLIB_DECL auto begin() const -> iterator
        {
            return iterator(ReferenceWrapper(*this));
        }

        CERBLIB_DECL static auto end() -> CharT
        {
            return cast('\0');
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

    template<CharacterLiteral CharT>
    struct GeneratorForText<CharT>::iterator
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

        constexpr iterator() = default;
        constexpr explicit iterator(CharT chr) : current_char(chr)
        {}
        constexpr explicit iterator(ReferenceWrapper<const GeneratorForText> gen)
          : generator(gen.get()), current_char(generator.skipLayoutAndGiveChar())
        {}

    private:
        GeneratorForText generator{};
        CharT current_char{};
    };

    template<CharacterLiteral CharT>
    struct GeneratorForText<CharT>::referenced_iterator
    {
        constexpr auto operator++() -> referenced_iterator &
        {
            current_char = generator.skipLayoutAndGiveChar();
            return *this;
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

        constexpr auto operator==(referenced_iterator const &other) const -> bool
        {
            return current_char == other.current_char;
        }

        constexpr referenced_iterator() = default;
        constexpr explicit referenced_iterator(CharT chr) : current_char(chr)
        {}
        constexpr explicit referenced_iterator(ReferenceWrapper<GeneratorForText> gen)
          : generator(gen.get())
        {
            if (generator.getOffset() == 0) {
                current_char = generator.skipLayoutAndGiveChar();
            } else {
                current_char = generator.getCharAtCurrentOffset(-1);
            }
        }

    private:
        GeneratorForText &generator;
        CharT current_char{};
    };

}// namespace cerb::lex

namespace cerb::lex::test
{
    template<CharacterLiteral CharT>
    struct TextGenerator
    {
        using location_t = LocationInFile;
        using str_t = std::basic_string<CharT>;
        using str_view_t = BasicStringView<CharT>;

        template<std::integral U>
        CERBLIB_DECL static auto cast(U value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        constexpr TextGenerator() = default;
        constexpr explicit TextGenerator(
            str_view_t const &files_text, LocationInFile const &current_location = {})
          : location(current_location), text(files_text)
        {}

    private:
        location_t location{};
        str_t tabs_and_spaces{};
        str_view_t text{};
        str_view_t current_line{};
    };
}// namespace cerb::lex::test

#endif /* CERBERUS_TEXT_GENERATOR_HPP */
