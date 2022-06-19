#ifndef CERBERUS_TEXT_GENERATOR_HPP
#define CERBERUS_TEXT_GENERATOR_HPP

#include <cerberus/char.hpp>
#include <cerberus/text_scan_assistance/generator_modules/line_tracker.hpp>
#include <cerberus/text_scan_assistance/generator_modules/tabs_and_spaces_saver.hpp>
#include <cerberus/text_scan_assistance/location_in_file.hpp>
#include <cerberus/text_scan_assistance/scan_api_modules/skip_mode.hpp>
#include <cerberus/text_scan_assistance/text_exception.hpp>
#include <string>

namespace cerb::text
{
    CERBERUS_EXCEPTION(TextGeneratorError, BasicTextScanningException);

    template<CharacterLiteral CharT, CharacterLiteral FileNameT>
    class GeneratorForText : public LocationInFile<FileNameT>
    {
        using char_enum = lex::CharEnum<CharT>;
        using iterator = GetIteratorType<BasicStringView<CharT>>;
        using line_watcher_t = gen::LineWatcher<CharT, FileNameT>;
        using tabs_and_spaces_saver_t = gen::TabsAndSpacesSaver<CharT>;

        friend line_watcher_t;

        CERBLIB_DECL auto currentBegin() const -> iterator
        {
            return text.begin() + getOffset();
        }

        CERBLIB_DECL auto currentEnd() const -> iterator
        {
            return text.end();
        }

    public:
        CERBLIB_LOCATION_IN_FILE_ACCESS(FileNameT);

        CERBLIB_DECL auto isInitialized() const -> bool
        {
            return initialized;
        }

        CERBLIB_DECL auto getText() const -> BasicStringView<CharT> const &
        {
            return text;
        }

        CERBLIB_DECL auto getCurrentLine() const -> BasicStringView<CharT> const &
        {
            return line_watcher.get();
        }

        CERBLIB_DECL auto getTabsAndSpaces() const -> std::basic_string<CharT> const &
        {
            return tabs_and_spaces.get();
        }

        CERBLIB_DECL auto getRestOfTheText() const -> BasicStringView<CharT>
        {
            return { currentBegin(), currentEnd() };
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
            using namespace lex;

            while (isLayout(getRawChar())) {
                // empty loop
            }

            return getCurrentChar();
        }

        constexpr auto skip(size_t times, SkipMode mode = SkipMode::RAW_CHARS) -> void
        {
            for (size_t i = 0; i != times; ++i) {
                if (mode == CLEAN_CHARS) {
                    getCleanChar();
                } else {
                    getRawChar();
                }
            }
        }

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
            line_watcher.forceUpdate(*this);
        }

    private:
        CERBLIB_DECL auto at(size_t index) const -> CharT
        {
            return text[index];
        }

        constexpr auto processFirstRawChar() -> void
        {
            initialized = true;
            tabs_and_spaces.newChar(getCurrentChar());
        }

        constexpr auto processRawChar() -> void
        {
            updateLocationToTheNextChar();
            line_watcher.update(*this);
        }

        constexpr auto updateLocationToTheNextChar() -> void
        {
            auto new_char = getCurrentChar(1);

            tabs_and_spaces.newChar(new_char);

            if (new_char == char_enum::NewLine) {
                processNewLine();
            } else {
                processNewChar();
            }
        }

        constexpr auto processNewLine() -> void
        {
            location_t::newLine();
        }

        constexpr auto processNewChar() -> void
        {
            location_t::newChar();
        }

        CERBLIB_DECL auto calculateRealOffset(ssize_t offset) const -> size_t
        {
            constexpr size_t high_offset = std::numeric_limits<ssize_t>::max();

            auto real_offset = static_cast<ssize_t>(getOffset()) + offset;
            return real_offset < 0 ? high_offset : static_cast<size_t>(real_offset);
        }

        CERBLIB_DECL auto isCurrentCharEoF() const -> bool
        {
            auto chr = at(getOffset());
            return lex::isEoF(chr);
        }

        constexpr auto checkForkingBorders(size_t from, size_t to) const -> void
        {
            auto offset = getOffset();

            auto is_from_greater_than_to = from > to;
            auto is_begin_out_of_range = from + offset >= text.size();
            auto is_end_out_of_range = to + offset >= text.size();

            if (logicalOr(is_begin_out_of_range, is_end_out_of_range, is_from_greater_than_to)) {
                throw TextGeneratorError("Unable to fork generator with given borders");
            }
        }

        tabs_and_spaces_saver_t tabs_and_spaces{};
        line_watcher_t line_watcher{};
        BasicStringView<CharT> text{};
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
