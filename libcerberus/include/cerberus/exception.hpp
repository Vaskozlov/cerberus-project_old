#ifndef CERBERUS_EXCEPTION_HPP
#define CERBERUS_EXCEPTION_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/type.hpp>
#include <exception>
#include <stdexcept>
#include <string_view>

#define CERBERUS_EXCEPTION(name, base_exception)                                                   \
    struct name : base_exception                                                                   \
    {                                                                                              \
        static_assert(std::is_base_of_v<CerberusException, base_exception>);                       \
                                                                                                   \
        name() noexcept = default;                                                                 \
                                                                                                   \
        explicit name(std::string_view exception_message) : base_exception(exception_message)      \
        {}                                                                                         \
    }

namespace cerb
{
    struct CerberusException : public std::exception
    {
        CerberusException() noexcept = default;

        explicit CerberusException(std::string_view exception_message) : message(exception_message)
        {}

        [[nodiscard]] auto what() const noexcept -> char const * override
        {
            return message.data();
        }

    protected:
        std::string_view message{};
    };

    template<typename T>
    concept CerberusExceptionType = std::is_base_of_v<CerberusException, T>;
}// namespace cerb

#endif /* CERBERUS_EXCEPTION_HPP */
