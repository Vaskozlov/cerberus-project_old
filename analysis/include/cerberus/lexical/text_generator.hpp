#ifndef CERBERUS_TEXT_GENERATOR_HPP
#define CERBERUS_TEXT_GENERATOR_HPP

#include <cerberus/enum.hpp>
#include <cerberus/lexical/location.hpp>
#include <cerberus/reference_wrapper.hpp>
#include <string>

namespace cerb::lex
{
    template<CharacterLiteral CharT = char>
    class TextGenerator
    {
        using location_t = LocationInFile;

        using str_t = std::basic_string<CharT>;
        using str_view_t = BasicStringView<CharT>;

        template<std::integral U>
        CERBLIB_DECL static auto cast2Char(U value) -> CharT
        {
            return static_cast<CharT>(value);
        }

    public:
        CERBLIB_DECL auto getOffset() const -> size_t
        {
            return location.getOffset();
        }

        CERBLIB_DECL auto getCharNumber() const -> size_t
        {
            return location.getCharacterInLine();
        }

        CERBLIB_DECL auto getLineNumber() const -> size_t
        {
            return location.getLine();
        }

        CERBLIB_DECL auto getFilename() const -> str_view_t const &
        {
            return location.getFilename();
        }

        CERBLIB_DECL auto getLineInText() const -> str_view_t const &
        {
            return line_of_text;
        }

        CERBLIB_DECL auto getLocation() const -> location_t const &
        {
            return location;
        }

        CERBLIB_DECL auto getTabsAndSpaces() const -> str_t const &
        {
            return tabs_and_spaces;
        }

        CERBLIB_DECL auto getCurrentChar() const -> CharT
        {
            if (not initialized) {
                return cast2Char('\0');
            }

            return text[getOffset()];
        }

        CERBLIB_DECL auto getRawChar() -> CharT
        {
            auto offset = getOffset();

            if (not initialized) {
                initialized = true;
                tryToAppendTabOrSpace();
            } else {
                if (text[offset] == cast2Char('\0')) {
                    return cast2Char('\0');
                }

                callFunctionForNewChar();
                updateLineIfNeed();
                offset = getOffset();
            }

            return text[offset];
        }

        CERBLIB_DECL auto getCharWithoutLayout() -> CharT
        {
            while (isLayout(getRawChar())) {
                // empty loop body
            }

            return text[getOffset()];
        }

        constexpr TextGenerator() = default;
        constexpr explicit TextGenerator(
            str_view_t const &text_data,
            str_view_t const &filename = {})
          : location(filename, 1, 1, 0), text(text_data)
        {
            setLineOfText();
        }

    private:
        constexpr auto updateLineIfNeed() -> void
        {
            if (text[getOffset() - 1] == cast2Char('\n')) {
                setLineOfText();
            }
        }

        constexpr auto tryToAppendTabOrSpace() -> void
        {
            auto offset = getOffset();
            auto chr = text[offset];

            if (logicalOr(chr == cast2Char('\t'), chr == cast2Char(' '))) {
                tabs_and_spaces.push_back(chr);
            }
        }

        constexpr auto callFunctionForNewChar() -> void
        {
            auto offset = getOffset() + 1;

            if (text[offset] == cast2Char('\n')) {
                processNewLine();
            } else {
                processNewChar();
            }
        }

        constexpr auto processNewLine() -> void
        {
            location.newLine();
            clearTabsAndSpaces();
        }

        constexpr auto processNewChar() -> void
        {
            tryToClearTabsAndSpaces();
            location.newChar();
            tryToAppendTabOrSpace();
        }

        constexpr auto tryToClearTabsAndSpaces() -> void
        {
            if (needToClearTabsAndSpaces()) {
                clearTabsAndSpaces();
            }
        }

        constexpr auto clearTabsAndSpaces() -> void
        {
            tabs_and_spaces.clear();
        }

        CERBLIB_DECL auto needToClearTabsAndSpaces() const -> bool
        {
            return logicalAnd(
                not tabs_and_spaces.empty(), text[getOffset()] != cast2Char('\t'),
                text[getOffset()] != cast2Char(' '));
        }

        constexpr auto setLineOfText() -> void
        {
            auto offset = getOffset();
            auto begin_of_line = text.begin() + offset;
            auto end_of_line = text.begin() + text.find(cast2Char('\n'), offset);

            line_of_text = { begin_of_line, end_of_line };
        }

        location_t location{};
        str_t tabs_and_spaces{};
        str_view_t text{};
        str_view_t line_of_text{};
        bool initialized{ false };
    };
}// namespace cerb::lex

#endif /* CERBERUS_TEXT_GENERATOR_HPP */
