#ifndef CERBERUS_ENUM_HPP
#define CERBERUS_ENUM_HPP

#include <cerberus/bit.hpp>
#include <cerberus/cerberus.hpp>

#define CERBERUS_ENUM(name, type, ...)                                                             \
    class CERBLIB_TRIVIAL name                                                                     \
    {                                                                                              \
        enum struct LocalEnum : type                                                               \
        {                                                                                          \
            __VA_ARGS__                                                                            \
        };                                                                                         \
                                                                                                   \
        CERBLIB_DECL static auto cast(type value2cast) -> LocalEnum                                \
        {                                                                                          \
            return static_cast<LocalEnum>(value2cast);                                             \
        }                                                                                          \
                                                                                                   \
        CERBLIB_DECL static auto cast(LocalEnum value2cast) -> type                                \
        {                                                                                          \
            return static_cast<type>(value2cast);                                                  \
        }                                                                                          \
                                                                                                   \
    public:                                                                                        \
        CERBLIB_DECL auto get() const -> type                                                      \
        {                                                                                          \
            return cast(value);                                                                    \
        }                                                                                          \
                                                                                                   \
        constexpr auto operator&=(name other) -> name &                                            \
        {                                                                                          \
            value = cast(get() & other.get());                                                     \
            return *this;                                                                          \
        }                                                                                          \
                                                                                                   \
        constexpr auto operator&=(type other) -> name &                                            \
        {                                                                                          \
            value = cast(get() & other);                                                           \
            return *this;                                                                          \
        }                                                                                          \
                                                                                                   \
        constexpr auto operator|=(name other) -> name &                                            \
        {                                                                                          \
            value = cast(get() | other.get());                                                     \
            return *this;                                                                          \
        }                                                                                          \
                                                                                                   \
        constexpr auto operator|=(type other) -> name &                                            \
        {                                                                                          \
            value = cast(get() | other);                                                           \
            return *this;                                                                          \
        }                                                                                          \
                                                                                                   \
        CERBLIB_DECL auto operator&(name other) const -> name                                      \
        {                                                                                          \
            return name(get() & other.get());                                                      \
        }                                                                                          \
                                                                                                   \
        CERBLIB_DECL auto operator&(type other) const -> name                                      \
        {                                                                                          \
            return name(get() & other);                                                            \
        }                                                                                          \
                                                                                                   \
        CERBLIB_DECL auto operator|(name other) const -> name                                      \
        {                                                                                          \
            return name(get() | other.get());                                                      \
        }                                                                                          \
                                                                                                   \
        CERBLIB_DECL auto operator|(type other) const -> name                                      \
        {                                                                                          \
            return name(get() | other);                                                            \
        }                                                                                          \
                                                                                                   \
        CERBLIB_DECL auto isSet(name other) const -> bool                                          \
        {                                                                                          \
            return (get() & other.get()) == other.get();                                           \
        }                                                                                          \
                                                                                                   \
        CERBLIB_DECL auto isSet(type other) const -> bool                                          \
        {                                                                                          \
            return (get() & other) == other;                                                       \
        }                                                                                          \
                                                                                                   \
        constexpr auto clearFlag(name other) -> void                                               \
        {                                                                                          \
            value = cast(get() & (~other.get()));                                                  \
        }                                                                                          \
                                                                                                   \
        constexpr auto clearFlag(type other) -> void                                               \
        {                                                                                          \
            value = cast(get() & (~other));                                                        \
        }                                                                                          \
                                                                                                   \
        constexpr auto operator<=>(name const &other) const = default;                             \
                                                                                                   \
        name() = default;                                                                          \
        constexpr name(type value2set) : value(cast(value2set))                                    \
        {}                                                                                         \
                                                                                                   \
        constexpr static type __VA_ARGS__;                                                         \
                                                                                                   \
    private:                                                                                       \
        LocalEnum value;                                                                           \
    }

#endif /* CERBERUS_ENUM_HPP */
