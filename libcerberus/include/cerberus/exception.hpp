#ifndef CERBERUS_EXCEPTION_HPP
#define CERBERUS_EXCEPTION_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/type.hpp>
#include <exception>
#include <stdexcept>
#include <string_view>

#define CERBERUS_EXCEPTION(name, base_exception)                                                   \
    struct name : public base_exception                                                            \
    {                                                                                              \
        static_assert(cerb::ExceptionType<base_exception>);                                        \
                                                                                                   \
        name() noexcept = default;                                                                 \
                                                                                                   \
        explicit name(std::string_view t_message) : message(t_message)                             \
        {}                                                                                         \
                                                                                                   \
        [[nodiscard]] auto what() const noexcept -> char const * override                          \
        {                                                                                          \
            return message.data();                                                                 \
        }                                                                                          \
                                                                                                   \
    private:                                                                                       \
        std::string_view message{};                                                                \
    }

#endif /* CERBERUS_EXCEPTION_HPP */
