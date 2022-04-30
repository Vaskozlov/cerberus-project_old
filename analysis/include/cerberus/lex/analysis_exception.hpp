#ifndef CERBERUS_ANALYSIS_EXCEPTION_HPP
#define CERBERUS_ANALYSIS_EXCEPTION_HPP

#include <cerberus/exception.hpp>
#include <cerberus/format/format.hpp>
#include <cerberus/lex/generator_for_text.hpp>

namespace cerb
{
    template<CharacterLiteral CharT>
    struct AnalysisException : public std::exception
    {
        AnalysisException() = default;

        explicit AnalysisException(std::string_view exception_message) : message(exception_message)
        {}

        explicit constexpr AnalysisException(
            BasicStringView<char> const &exception_message,
            lex::GeneratorForText<CharT> const &generator)
          : message(fmt::format<CharT>(
                "Analysis error occurred: {}. File: {}, line: {}, char: {}\n{}", exception_message,
                generator.getFilename(), generator.getLine(), generator.getCharPosition(),
                generator.getCurrentLine()))
        {}

        [[nodiscard]] auto what() const noexcept -> char const * override
        {
            if constexpr (std::is_same_v<char, CharT>) {
                return message.c_str();
            } else {
                return "Unable to show message";
            }
        }

    private:
        std::basic_string<CharT> message{};
    };
}// namespace cerb

#endif /* CERBERUS_ANALYSIS_EXCEPTION_HPP */
