#ifndef CERBERUS_TEXT_GENERATOR_HPP
#define CERBERUS_TEXT_GENERATOR_HPP

#include <cerberus/enum.hpp>
#include <cerberus/lexical/file_location.hpp>
#include <cerberus/reference_wrapper.hpp>
#include <string>

namespace cerb::lex
{
    template<CharacterLiteral CharT = char>
    class TextGenerator
    {
        using location_t = LocationInFile;

        using chars_enum = CharsEnum<CharT>;
        using str_t = std::basic_string<CharT>;
        using str_view_t = BasicStringView<CharT>;

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

        CERBLIB_DECL auto getCurrentChar(ssize_t offset = 0) const -> CharT
        {
            if (not initialized) {
                return chars_enum::EoF;
            }

            auto real_offset = static_cast<ssize_t>(getOffset()) + offset;
            return text[static_cast<size_t>(real_offset)];
        }

        CERBLIB_DECL auto getCurrentString() const -> str_view_t
        {
            return { text.begin() + getOffset(), text.end() };
        }

        constexpr auto getRawChar() -> CharT
        {
            auto offset = getOffset();

            if (not initialized) {
                initialized = true;
                tryToAppendTabOrSpace();
            } else {
                if (isEndOfInput(text[offset])) {
                    return chars_enum::EoF;
                }

                updateLocation();
                tryToUpdateLine();
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

        constexpr auto skipLayout() -> void
        {
            while (isLayout(getCurrentChar(1))) {
                getRawChar();
            }
        }

        constexpr TextGenerator() = default;
        constexpr explicit TextGenerator(
            str_view_t const &text_data,
            cerb::string_view const &filename = {})
          : location(filename, 1, 1, 0), text(text_data)
        {
            setCurrentWorkingLine();
        }

    private:
        constexpr auto tryToUpdateLine() -> void
        {
            if (text[getOffset() - 1] == chars_enum::NewLine) {
                setCurrentWorkingLine();
            }
        }

        constexpr auto tryToAppendTabOrSpace() -> void
        {
            auto offset = getOffset();
            auto chr = text[offset];

            if (logicalOr(chr == chars_enum::Tab, chr == chars_enum::Space)) {
                tabs_and_spaces.push_back(chr);
            }
        }

        constexpr auto updateLocation() -> void
        {
            auto offset = getOffset() + 1;

            if (text[offset] == chars_enum::NewLine) {
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
                not tabs_and_spaces.empty(), text[getOffset()] != chars_enum::Tab,
                text[getOffset()] != chars_enum::Space);
        }

        constexpr auto setCurrentWorkingLine() -> void
        {
            auto offset = getOffset();
            auto begin_of_line = text.begin() + offset;
            auto end_of_line = text.begin() + text.find(chars_enum::NewLine, offset);

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
