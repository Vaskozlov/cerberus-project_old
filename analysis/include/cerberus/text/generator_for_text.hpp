#ifndef CERBERUS_TEXT_GENERATOR_HPP
#define CERBERUS_TEXT_GENERATOR_HPP

#include <cerberus/lex/char.hpp>
#include <cerberus/text/location_in_file.hpp>
#include <cerberus/text/text_exception.hpp>
#include <string>

namespace cerb::text
{
    CERBERUS_EXCEPTION(TextGeneratorError, BasicTextAnalysisException);

    template<CharacterLiteral CharT, CharacterLiteral FileNameT = char>
    struct GeneratorForText
    {
        CERBLIB_DECL auto isInitialized() const -> bool
        {
            return initialized;
        }

        CERBLIB_DECL auto getLocation() const -> LocationInFile<FileNameT> const &
        {
            return location;
        }

        CERBLIB_DECL auto getTextOffset() const -> size_t
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
            return {text.begin() + getTextOffset(), text.end() };
        }

        CERBLIB_DECL auto getCurrentChar(ssize_t offset = 0) const -> CharT
        {
            using namespace lex;

            if (not initialized) {
                return CharEnum<CharT>::EoF;
            }

            auto real_offset = calculateRealOffset(offset);
            return real_offset >= text.size() ? CharEnum<CharT>::EoF : text[real_offset];
        }

        constexpr auto getRawChar() -> CharT
        {
            using namespace lex;

            if (isEoF(text[getTextOffset()])) {
                return CharEnum<CharT>::EoF;
            }

            if (not initialized) {
                processFirstRawChar();
            } else {
                processRawChar();
            }

            return getCurrentChar();
        }

        constexpr auto getCleanChar() -> CharT
        {
            while (lex::isLayout(getRawChar())) {
                // empty loop
            }

            return getCurrentChar();
        }

        constexpr auto skip(size_t times) -> void
        {
            for (size_t i = 0; i < times; ++i) {
                getRawChar();
            }
        }

        GeneratorForText() = default;

        constexpr explicit GeneratorForText(
            BasicStringView<CharT> const &file_content,
            BasicStringView<FileNameT> const &name_of_file = {})
          : location(name_of_file), text(file_content)
        {
            updateCurrentLine();
        }

    private:
        constexpr auto processFirstRawChar() -> void
        {
            initialized = true;
            tryToAddTabOrSpace();
        }

        constexpr auto processRawChar() -> void
        {
            updateLocationToTheNextChar();
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
            auto offset = getTextOffset() - 1;
            return text[offset] == lex::CharEnum<CharT>::NewLine;
        }

        constexpr auto updateCurrentLine() -> void
        {
            auto offset = location.offset();
            auto text_begin = text.begin();
            auto begin_of_line = text_begin + offset;
            size_t line_size = text.find(lex::CharEnum<CharT>::NewLine, offset) - offset;

            current_line = { begin_of_line, line_size };
        }

        constexpr auto updateLocationToTheNextChar() -> void
        {
            auto future_offset = getTextOffset() + 1;

            if (text[future_offset] == lex::CharEnum<CharT>::NewLine) {
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
            using namespace lex;

            auto offset = getTextOffset();
            auto chr = text[offset];

            if (isTabOrSpace(chr)) {
                tabs_and_spaces.push_back(chr);
            }
        }

        CERBLIB_DECL auto needToClearTabsAndSpaces() const -> bool
        {
            using namespace lex;
            auto offset = getTextOffset();

            return logicalAnd(not tabs_and_spaces.empty(), not isTabOrSpace(text[offset]));
        }

        CERBLIB_DECL auto calculateRealOffset(ssize_t offset) const -> size_t
        {
            auto real_offset = static_cast<ssize_t>(getTextOffset()) + offset;
            checkOffset(real_offset);

            return static_cast<size_t>(real_offset);
        }

        CERBLIB_DECL static auto isTabOrSpace(CharT chr) -> bool
        {
            using namespace lex;

            return logicalOr(chr == CharEnum<CharT>::Tab, chr == CharEnum<CharT>::Space);
        }

        constexpr auto checkOffset(ssize_t offset) const -> void
        {
            if (offset < 0) {
                throw TextGeneratorError("Unable to access char at given offset!");
            }
        }

        LocationInFile<FileNameT> location{};
        std::basic_string<CharT> tabs_and_spaces{};
        BasicStringView<CharT> text{};
        BasicStringView<CharT> current_line{};
        bool initialized{ false };
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct GeneratorForText<char>;
    extern template struct GeneratorForText<char8_t>;
    extern template struct GeneratorForText<char16_t>;
    extern template struct GeneratorForText<char32_t>;
    extern template struct GeneratorForText<wchar_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::text

#endif /* CERBERUS_TEXT_GENERATOR_HPP */
