#ifndef CERBERUS_TEXT_GENERATOR_HPP
#define CERBERUS_TEXT_GENERATOR_HPP

#include <cerberus/enum.hpp>
#include <cerberus/lexical/location.hpp>
#include <cerberus/reference_wrapper.hpp>
#include <functional>
#include <future>
#include <optional>
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

        CERBLIB_DECL auto getCharAtCurrentOffset(ssize_t offset_for_offset = 0) const -> CharT
        {
            return text_data[static_cast<size_t>(
                static_cast<ssize_t>(getOffset()) + offset_for_offset)];
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
    class TextGenerator
    {
        using future_t = std::future<void>;
        using location_t = LocationInFile;
        using str_t = std::basic_string<CharT>;
        using str_view_t = BasicStringView<CharT>;

        CERBERUS_ENUM(FUTURE_STATE, u16, UNINITIALIZED = 0, RAW_CHAR = 1, CLEAN_CHAR = 2);// NOLINT

        template<std::integral U>
        CERBLIB_DECL static auto cast2Char(U value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        struct GeneratorState
        {
            CERBLIB_DECL auto getOffset() const -> size_t
            {
                return location.getOffset();
            }

            constexpr GeneratorState() = default;
            constexpr explicit GeneratorState(location_t const &new_location)
              : location(new_location)
            {}

            location_t location{};
            str_t tabs_and_spaces{};
            str_view_t line_in_text{};
        };

    public:
        constexpr auto getRawChar() -> CharT
        {
            if (future_state == FUTURE_STATE::UNINITIALIZED) {
                next_state = current_state;
            } else if (future_state == FUTURE_STATE::CLEAN_CHAR) {
                nextRawChar(text, &current_state);
                next_state = current_state;
            } else {
                future_predictor.get();
                current_state = next_state;
            }

            dealWithRawFuture();
            return getChar(text, current_state);
        }

        constexpr auto getCleanChar() -> CharT
        {
            if (future_state == FUTURE_STATE::UNINITIALIZED) {
                skipLayout(text, current_state);
                next_state = current_state;
            } else if (future_state == FUTURE_STATE::RAW_CHAR) {
                skipLayout(text, current_state);
                nextRawChar(text, &current_state);
                next_state = current_state;
            } else {
                future_predictor.get();
                current_state = next_state;
            }

            dealWithCleanFuture();
            return getChar(text, current_state);
        }

        constexpr TextGenerator() = default;
        constexpr explicit TextGenerator(
            str_view_t const &files_text, location_t const &current_location = {})
          : current_state(current_location), text(files_text)
        {}

    private:
        CERBLIB_DECL static auto
            getChar(str_view_t const &text, GeneratorState const &state, ssize_t extra_offset = 0)
                -> CharT
        {
            auto offset =
                static_cast<size_t>(static_cast<ssize_t>(state.getOffset()) + extra_offset);
            return text[offset];
        }

        constexpr auto dealWithRawFuture() -> void
        {
            future_predictor = std::async(std::launch::async, nextRawChar, text, &next_state);
            future_state = FUTURE_STATE::RAW_CHAR;
        }

        constexpr static auto nextRawChar(str_view_t const &text, GeneratorState *state) -> void
        {
            callNewCharFunction(text, *state);
        }

        constexpr static auto callNewCharFunction(str_view_t const &text, GeneratorState &state)
            -> void
        {
            auto offset = state.getOffset();

            switch (text[offset]) {
            case cast2Char('\0'):
                break;

            case cast2Char('\n'):
                state.location.newLine();
                break;

            case cast2Char('\t'):
            case cast2Char(' '):
                state.tabs_and_spaces.push_back(text[offset]);
                [[fallthrough]];

            default:
                state.location.newChar();
            }
        }

        constexpr auto dealWithCleanFuture() -> void
        {
            future_predictor = std::async(std::launch::async, nextCleanChar, text, &next_state);
            future_state = FUTURE_STATE::CLEAN_CHAR;
        }

        constexpr static auto nextCleanChar(str_view_t const &text, GeneratorState *state) -> void
        {
            skipLayout(text, *state);
            callNewCharFunction(text, *state);
        }

        constexpr static auto skipLayout(str_view_t const &text, GeneratorState &state) -> void
        {
            while (isLayout(getChar(text, state, 1))) {
                callNewCharFunction(text, state);
            }
        }

        constexpr auto tryToClearTabsAndSpaces(GeneratorState &state) -> void
        {
            if (needToClearTabsAndSpaces(state)) {
                clearTabsAndSpaces();
            }
        }

        constexpr static auto clearTabsAndSpaces(GeneratorState &state) -> void
        {
            state.tabs_and_spaces.clear();
        }

        CERBLIB_DECL auto needToClearTabsAndSpaces(GeneratorState const &state) const -> bool
        {
            return logicalAnd(
                not state.tabs_and_spaces.empty(), getChar(text, state, -1) != cast2Char('\t'),
                getChar(text, state, -1) != cast2Char(' '));
        }

        constexpr auto setLineInState(GeneratorState &state) const -> void
        {
            auto offset = state.getOffset();
            auto begin_of_line = text.begin() + offset;
            auto end_of_line = text.begin() + text.find(cast2Char('\n'), offset);
            state.line_in_text = { begin_of_line, end_of_line };
        }

        GeneratorState current_state{};
        GeneratorState next_state{};
        future_t future_predictor{};
        str_view_t text{};
        FUTURE_STATE future_state{ FUTURE_STATE::UNINITIALIZED };
    };
}// namespace cerb::lex::test

#endif /* CERBERUS_TEXT_GENERATOR_HPP */
