#ifndef CERBERUS_ANALYSIS_EXCEPTION_HPP
#define CERBERUS_ANALYSIS_EXCEPTION_HPP

#include <cerberus/exception.hpp>
#include <cerberus/format/format.hpp>
#include <cerberus/lex/generator_for_text.hpp>
#include <cerberus/lex/string_reducer.hpp>
#include <cerberus/reference_wrapper.hpp>

#define CERBERUS_ANALYSIS_EXCEPTION(name, CharT)                                                   \
    struct name : public AnalysisException<CharT>                                                  \
    {                                                                                              \
        constexpr name(                                                                            \
            cerb::BasicStringView<char> const &exception_message,                                  \
            cerb::lex::GeneratorForText<CharT> const &generator)                                   \
          : AnalysisException<CharT>(exception_message, generator)                                 \
        {}                                                                                         \
    }

namespace cerb
{
    template<CharacterLiteral CharT>
    struct AnalysisException : public std::exception
    {
        CERBLIB_DECL auto getLocation() const -> lex::LocationInFile<char> const &
        {
            return text_generator.getLocation();
        }

        CERBLIB_DECL auto getOffset() const -> size_t
        {
            return text_generator.getOffset();
        }

        CERBLIB_DECL auto getLine() const -> size_t
        {
            return text_generator.getLine();
        }

        CERBLIB_DECL auto getCharPosition() const -> size_t
        {
            return text_generator.getCharPosition();
        }

        CERBLIB_DECL auto getFilename() const -> BasicStringView<char> const &
        {
            return text_generator.getFilename();
        }

        CERBLIB_DECL auto getTabsAndSpaces() const -> std::basic_string<CharT> const &
        {
            return text_generator.getTabsAndSpaces();
        }

        CERBLIB_DECL auto getText() const -> BasicStringView<CharT> const &
        {
            return text_generator.getText();
        }

        CERBLIB_DECL auto getCurrentLine() const -> BasicStringView<CharT> const &
        {
            return text_generator.getCurrentLine();
        }

        CERBLIB_DECL auto getRestOfTheText() const -> BasicStringView<CharT>
        {
            return text_generator.getRestOfTheText();
        }

        AnalysisException() = default;

        explicit constexpr AnalysisException(
            BasicStringView<char> const &exception_message,
            lex::GeneratorForText<CharT> const &generator)
          : text_generator(generator), unable_to_show_message(getNonCharErrorMessage()),
            message(getErrorMessage(exception_message))
        {}

        [[nodiscard]] auto what() const noexcept -> char const * override
        {
            if constexpr (std::is_same_v<char, CharT>) {
                return message.c_str();
            } else {
                return unable_to_show_message.c_str();
            }
        }

    private:
        static auto getNonCharErrorMessage() -> std::string
        {
            if constexpr (not std::is_same_v<char, CharT>) {
                return fmt::format<char>(
                    "Unable to show "
                    "error message, because AnalysisException character type is {}",
                    typeid(CharT).name());
            } else {
                return "";
            }
        }

        auto getErrorMessage(BasicStringView<char> const &exception_message)
            -> std::basic_string<CharT>
        {
            lex::StringReducer<CharT> reducer{ text_generator };

            std::basic_string<CharT> result = fmt::format<CharT>(
                "Analysis error occurred: {} File: {}, line: {}, char: {}\n{}\n",
                exception_message, getFilename(), getLine(), getCharPosition(),
                reducer.getResult());

            result.resize(
                result.size() + reducer.getErrorPositionAfterReducing(), static_cast<CharT>(' '));
            result.push_back(static_cast<CharT>('^'));

            return result;
        }

        lex::GeneratorForText<CharT> const &text_generator;
        std::string unable_to_show_message{};
        std::basic_string<CharT> message{};
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct AnalysisException<char>;
    extern template struct AnalysisException<char8_t>;
    extern template struct AnalysisException<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb

#endif /* CERBERUS_ANALYSIS_EXCEPTION_HPP */
