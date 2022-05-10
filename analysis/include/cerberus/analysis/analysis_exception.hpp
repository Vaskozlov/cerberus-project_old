#ifndef CERBERUS_ANALYSIS_EXCEPTION_HPP
#define CERBERUS_ANALYSIS_EXCEPTION_HPP

#include <cerberus/analysis/analysis_basic_exception.hpp>
#include <cerberus/format/format.hpp>
#include <cerberus/reference_wrapper.hpp>
#include <cerberus/text/generator_for_text.hpp>
#include <cerberus/text/string_reducer.hpp>

#define CERBERUS_ANALYSIS_EXCEPTION(name, CharT, from)                                             \
    struct name : public cerb::analysis::AnalysisException<CharT, from>                            \
    {                                                                                              \
        constexpr name(                                                                            \
            cerb::BasicStringView<char> const &exception_message,                                  \
            cerb::text::GeneratorForText<CharT> const &generator)                                  \
          : cerb::analysis::AnalysisException<CharT, from>(exception_message, generator)           \
        {}                                                                                         \
    }

namespace cerb::analysis
{
    template<CharacterLiteral CharT, ExceptionType ExceptionT = BasicAnalysisException>
    struct AnalysisException : public ExceptionT
    {
        CERBLIB_DECL auto getLocation() const -> text::LocationInFile<char> const &
        {
            return text_generator.getLocation();
        }

        CERBLIB_DECL auto getOffset() const -> size_t
        {
            return text_generator.getTextOffset();
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

        AnalysisException() noexcept = default;

        explicit constexpr AnalysisException(
            BasicStringView<char> const &exception_message,
            text::GeneratorForText<CharT> const &generator)
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
            using namespace string_view_literals;

            if constexpr (not std::is_same_v<char, CharT>) {
                static const std::string message = fmt::format<char>(
                    "Unable to show "
                    "error message, because AnalysisException character type is {}"_sv,
                    typeid(CharT).name());
                return message;
            } else {
                return "";
            }
        }

        auto getErrorMessage(BasicStringView<char> const &exception_message)
            -> std::basic_string<CharT>
        {
            using namespace string_view_literals;

            text::StringReducer<CharT> reducer{ text_generator };

            std::basic_string<CharT> result = fmt::format<CharT>(
                "Analysis error occurred: {} File: {}, line: {}, char: {}\n{}\n"_sv,
                exception_message, getFilename(), getLine(), getCharPosition(),
                reducer.getReducedString());

            addArrowToTheMessage(result, reducer.getErrorPositionAfterReducing());

            return result;
        }

        static auto addArrowToTheMessage(std::basic_string<CharT> &result, size_t offset) -> void
        {
            result.resize(result.size() + offset, static_cast<CharT>(' '));
            result.push_back(static_cast<CharT>('^'));
        }

        text::GeneratorForText<CharT> const &text_generator;
        std::string unable_to_show_message{};
        std::basic_string<CharT> message{};
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct AnalysisException<char>;
    extern template struct AnalysisException<char8_t>;
    extern template struct AnalysisException<char16_t>;
    extern template struct AnalysisException<char32_t>;
    extern template struct AnalysisException<wchar_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::analysis

#endif /* CERBERUS_ANALYSIS_EXCEPTION_HPP */
