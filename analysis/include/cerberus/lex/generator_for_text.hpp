#ifndef CERBERUS_TEXT_GENERATOR_HPP
#define CERBERUS_TEXT_GENERATOR_HPP

#include <cerberus/lex/char.hpp>
#include <cerberus/lex/location_in_file.hpp>
#include <string>

namespace cerb::lex
{
    CERBERUS_EXCEPTION(TextGeneratorError);

    template<CharacterLiteral CharT, CharacterLiteral FileNameT = char>
    struct GeneratorForText
    {
        CERBLIB_DECL auto getLocation() const -> LocationInFile<FileNameT> const &
        {
            return location;
        }

        CERBLIB_DECL auto getOffset() const -> size_t
        {
            return location.offset();
        }

        CERBLIB_DECL auto getLine() const -> size_t
        {
            return location.line();
        }

        CERBLIB_DECL auto getCharPosition() const -> size_t
        {
            return location.charPosition();
        }

        CERBLIB_DECL auto getFilename() const -> BasicStringView<FileNameT> const &
        {
            return location.filename();
        }

        CERBLIB_DECL auto getTabsAndSpaces() const -> std::basic_string<CharT> const &
        {
            return tabs_and_spaces;
        }

        CERBLIB_DECL auto getText() const -> BasicStringView<CharT> const &
        {
            return text;
        }

        CERBLIB_DECL auto getCurrentLine() const -> BasicStringView<CharT> const &
        {
            return current_line;
        }

        CERBLIB_DECL auto getRestOfTheText() const -> BasicStringView<CharT>
        {
            return { text.begin() + getOffset(), text.end() };
        }

        CERBLIB_DECL auto getCurrentChar(ssize_t offset = 0) const -> CharT
        {
            if (not initialized) {
                return CharEnum<CharT>::EoF;
            }

            auto real_offset = static_cast<ssize_t>(getOffset()) + offset;
            checkOffset(real_offset);

            return text[static_cast<size_t>(real_offset)];
        }

        constexpr auto getRawChar() -> CharT
        {
            if (isEoF(text[getOffset()])) {
                return CharEnum<CharT>::EoF;
            }

            if (not initialized) {
                processFirstRawChar();
            } else {
                processRawChar();
            }

            return text[getOffset()];
        }

        constexpr auto getCleanChar() -> CharT
        {
            while (isLayout(getRawChar())) {
                // empty loop
            }

            return text[getOffset()];
        }

        GeneratorForText() = default;
        constexpr GeneratorForText(
            BasicStringView<CharT> const &file_content,
            BasicStringView<FileNameT> const &name_of_file)
          : location(name_of_file), text(file_content)
        {
            updateCurrentLine();
        }

    private:
        constexpr auto checkOffset(ssize_t offset) const -> void
        {
            if (logicalOr(offset > static_cast<ssize_t>(text.size()), offset < 0)) {
                throw TextGeneratorError("Unable to access char at given offset");
            }
        }

        constexpr auto processFirstRawChar() -> void
        {
            initialized = true;
            tryToAddTabOrSpace();
        }

        constexpr auto processRawChar() -> void
        {
            updateLocation();
            tryToUpdateLine();
        }

        constexpr auto tryToUpdateLine() -> void
        {
            if (needToUpdateLine()) {
                updateCurrentLine();
            }
        }

        CERBLIB_DECL auto needToUpdateLine() const -> bool
        {
            auto offset = getOffset() - 1;
            return text[offset] == CharEnum<CharT>::NewLine;
        }

        constexpr auto updateCurrentLine() -> void
        {
            auto offset = location.offset();
            auto text_begin = text.begin();
            auto begin_of_line = text_begin + offset;
            size_t line_size = text.find(CharEnum<CharT>::NewLine, offset) - offset;

            current_line = { begin_of_line, line_size };
        }

        constexpr auto updateLocation() -> void
        {
            auto future_offset = getOffset() + 1;

            if (text[future_offset] == CharEnum<CharT>::NewLine) {
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
            tryToAddTabOrSpace();
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

        constexpr auto tryToAddTabOrSpace() -> void
        {
            auto offset = getOffset();
            auto chr = text[offset];

            if (logicalOr(chr == CharEnum<CharT>::Tab, chr == CharEnum<CharT>::Space)) {
                tabs_and_spaces.push_back(chr);
            }
        }

        CERBLIB_DECL auto needToClearTabsAndSpaces() const -> bool
        {
            auto offset = getOffset();
            return logicalAnd(
                not tabs_and_spaces.empty(), text[offset] != CharEnum<CharT>::Tab,
                text[offset] != CharEnum<CharT>::Space);
        }

        LocationInFile<FileNameT> location{};
        std::basic_string<CharT> tabs_and_spaces{};
        BasicStringView<CharT> text{};
        BasicStringView<CharT> current_line{};
        bool initialized{ false };
    };
}// namespace cerb::lex

#endif /* CERBERUS_TEXT_GENERATOR_HPP */
