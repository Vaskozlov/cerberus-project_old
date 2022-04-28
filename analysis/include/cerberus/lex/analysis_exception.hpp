#ifndef CERBERUS_ANALYSIS_EXCEPTION_HPP
#define CERBERUS_ANALYSIS_EXCEPTION_HPP

#include <cerberus/exception.hpp>
#include <cerberus/lex/generator_for_text.hpp>
#include <fmt/format.h>

namespace cerb
{
    template<CharacterLiteral CharT>
    struct AnalysisException : public std::exception
    {
        AnalysisException() = default;

        explicit AnalysisException(std::string_view exception_message) : message(exception_message)
        {}

        explicit constexpr AnalysisException(
            std::string_view exception_message,
            lex::GeneratorForText<CharT> const &)
          : message(exception_message)
        {}

        [[nodiscard]] auto what() const noexcept -> char const * override
        {
            return message.c_str();
        }

    private:
        std::string message{};
    };
}// namespace cerb

#endif /* CERBERUS_ANALYSIS_EXCEPTION_HPP */
