#ifndef CERBERUS_EXCEPTION_HPP
#define CERBERUS_EXCEPTION_HPP

#include <exception>
#include <stdexcept>
#include <string_view>

#define CERBERUS_EXCEPTION(name)                                                                   \
    struct name : public std::exception                                                            \
    {                                                                                              \
        name() = default;                                                                          \
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
