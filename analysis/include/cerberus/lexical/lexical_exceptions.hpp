#ifndef CERBERUS_LEXICAL_EXCEPTIONS_HPP
#define CERBERUS_LEXICAL_EXCEPTIONS_HPP

#include <exception>
#include <string_view>

namespace cerb::lex {
    class DotItemParsingError : public std::exception
    {
        std::string_view message;

    public:
        DotItemParsingError() = default;

        explicit DotItemParsingError(std::string_view t_message) : message(t_message)
        {}

        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            return message.data();
        }
    };

    class DotItemNotASequenceError : public std::exception
    {
        std::string_view message;

    public:
        DotItemNotASequenceError() = default;

        explicit DotItemNotASequenceError(std::string_view t_message) : message(t_message)
        {}

        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            return message.data();
        }
    };
}// namespace cerb::lex

#endif /* CERBERUS_LEXICAL_EXCEPTIONS_HPP */
