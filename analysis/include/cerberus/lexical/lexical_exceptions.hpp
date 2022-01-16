#ifndef CERBERUS_LEXICAL_EXCEPTIONS_HPP
#define CERBERUS_LEXICAL_EXCEPTIONS_HPP

#include <exception>
#include <string_view>

namespace cerb::lex
{
    class ParsingError : public std::exception
    {
        std::string_view message{};

    public:
        ParsingError() = default;

        explicit ParsingError(std::string_view t_message) : message(t_message)
        {}

        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            return message.data();
        }
    };

    class SequenceError : public std::exception
    {
        std::string_view message{};

    public:
        SequenceError() = default;

        explicit SequenceError(std::string_view t_message) : message(t_message)
        {}

        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            return message.data();
        }
    };
}// namespace cerb::lex

#endif /* CERBERUS_LEXICAL_EXCEPTIONS_HPP */
