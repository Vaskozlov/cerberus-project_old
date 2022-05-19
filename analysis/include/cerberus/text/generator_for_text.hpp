#ifndef CERBERUS_TEXT_GENERATOR_HPP
#define CERBERUS_TEXT_GENERATOR_HPP

#include <cerberus/lex/char.hpp>
#include <cerberus/text/generator_modules/tabs_and_spaces_saver.hpp>
#include <cerberus/text/location_in_file.hpp>
#include <cerberus/text/text_exception.hpp>
#include <string>

namespace cerb::text
{
    CERBERUS_EXCEPTION(TextGeneratorError, BasicTextAnalysisException);

    template<CharacterLiteral CharT, CharacterLiteral FileNameT = char>
    class GeneratorForText : public LocationInFile<FileNameT>
    {
        using char_enum = lex::CharEnum<CharT>;
        using iterator = GetIteratorType<BasicStringView<CharT>>;
        using tabs_and_spaces_saver_t = gen::TabsAndSpacesSaver<CharT>;

        friend tabs_and_spaces_saver_t;

        CERBLIB_DECL auto currentTextBegin() const -> iterator
        {
            return text.begin() + charOffset();
        }

        CERBLIB_DECL auto currentTextEnd() const -> iterator
        {
            return text.end();
        }

    public:
        CERBLIB_LOCATION_IN_FILE_ACCESS(FileNameT);

        CERBLIB_DECL auto isInitialized() const -> bool
        {
            return initialized;
        }

        CERBLIB_DECL auto getTabsAndSpaces() const -> std::basic_string<CharT> const &
        {
            return tabs_and_spaces.get();
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
            return { currentTextBegin(), currentTextEnd() };
        }

        CERBLIB_DECL auto getCurrentChar(ssize_t offset = 0) const -> CharT
        {
            if (not initialized) {
                return char_enum::EoF;
            }

            auto real_offset = calculateRealOffset(offset);
            return real_offset >= text.size() ? char_enum::EoF : at(real_offset);
        }

        constexpr auto getRawChar() -> CharT
        {
            if (isCurrentCharEoF()) {
                return char_enum::EoF;
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

        template<bool SkipCleanChars = false>
        constexpr auto skip(size_t times) -> void
        {
            for (size_t i = 0; i != times; ++i) {
                if constexpr (SkipCleanChars) {
                    getCleanChar();
                } else {
                    getRawChar();
                }
            }
        }

        template<bool SkipCleanChars = false>
        CERBLIB_DECL auto fork(size_t from, size_t to) const -> GeneratorForText<CharT>
        {
            checkForkingBorders(from, to);

            GeneratorForText<CharT> forked_generator = *this;
            BasicStringView<CharT> &forked_text = forked_generator.text;

            forked_generator.skip(from);

            auto forked_text_begin = forked_text.begin();
            auto forked_text_length = to - from;

            forked_text = { forked_text_begin, forked_text_length };
            return forked_generator;
        }

        GeneratorForText() = default;

        constexpr explicit GeneratorForText(
            BasicStringView<CharT> const &file_content,
            BasicStringView<FileNameT> const &file_name = {})
          : location_t(file_name), text(file_content)
        {
            updateCurrentLine();
        }

    private:
        CERBLIB_DECL auto at(size_t index) const -> CharT
        {
            return text[index];
        }

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
            auto previous_offset = location_t::charOffset() - 1;
            return at(previous_offset) == char_enum::NewLine;
        }

        constexpr auto updateCurrentLine() -> void
        {
            auto offset = location_t::charOffset();
            size_t line_end = text.find(char_enum::NewLine, offset);
            size_t line_length = line_end - offset;

            current_line = { currentTextBegin(), line_length };
        }

        constexpr auto updateLocationToTheNextChar() -> void
        {
            auto future_offset = location_t::charOffset() + 1;

            if (at(future_offset) == char_enum::NewLine) {
                processNewLine();
            } else {
                processNewChar();
            }
        }

        constexpr auto processNewLine() -> void
        {
            location_t::newLine();
            tabs_and_spaces.clear();
        }

        constexpr auto processNewChar() -> void
        {
            tryToClearTabsAndSpaces();
            location_t::newChar();
            tryToAddTabOrSpace();
        }

        constexpr auto tryToClearTabsAndSpaces() -> void
        {
            tabs_and_spaces.tryToClear(getCurrentChar());
        }

        constexpr auto tryToAddTabOrSpace() -> void
        {
            tabs_and_spaces.tryToAdd(getCurrentChar());
        }

        CERBLIB_DECL auto calculateRealOffset(ssize_t offset) const -> size_t
        {
            auto real_offset = static_cast<ssize_t>(charOffset()) + offset;
            checkOffset(real_offset);

            return static_cast<size_t>(real_offset);
        }

        CERBLIB_DECL auto isCurrentCharEoF() const -> bool
        {
            auto chr = at(charOffset());
            return lex::isEoF(chr);
        }

        constexpr static auto checkOffset(ssize_t offset) -> void
        {
            if (offset < 0) {
                throw TextGeneratorError("Unable to access char at given charOffset!");
            }
        }

        constexpr auto checkForkingBorders(size_t from, size_t to) const -> void
        {
            auto offset = charOffset();

            if (logicalOr(from + offset >= text.size(), to + offset >= text.size(), from > to)) {
                throw TextGeneratorError("Unable to fork generator with given borders");
            }
        }

        tabs_and_spaces_saver_t tabs_and_spaces{};
        BasicStringView<CharT> text{};
        BasicStringView<CharT> current_line{};
        bool initialized{ false };
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template class GeneratorForText<char>;
    extern template class GeneratorForText<char8_t>;
    extern template class GeneratorForText<char16_t>;
    extern template class GeneratorForText<char32_t>;
    extern template class GeneratorForText<wchar_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::text

#endif /* CERBERUS_TEXT_GENERATOR_HPP */
