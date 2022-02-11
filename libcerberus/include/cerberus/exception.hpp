#ifndef CERBERUS_EXCEPTION_HPP
#define CERBERUS_EXCEPTION_HPP

#include <exception>
#include <string_view>

#define CERBERUS_EXCEPTION(name)                                                                   \
    class name : public std::exception                                                             \
    {                                                                                              \
        std::string_view message{};                                                                \
                                                                                                   \
    public:                                                                                        \
        name() = default;                                                                          \
                                                                                                   \
        explicit name(std::string_view t_message) : message(t_message)                             \
        {}                                                                                         \
                                                                                                   \
        [[nodiscard]] auto what() const noexcept -> char const * override                          \
        {                                                                                          \
            return message.data();                                                                 \
        }                                                                                          \
    }

#endif /* CERBERUS_EXCEPTION_HPP */
