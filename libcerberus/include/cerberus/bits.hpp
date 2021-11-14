#ifndef CERBERUS_BITS_HPP
#define CERBERUS_BITS_HPP

#include <cerberus/cerberus.hpp>

namespace cerb {
    /**
     * Union which holds target class and provides access to it's bytes
     * @tparam T
     */
    template<typename T>
    union CERBLIB_TRIVIAL ByteMask {
        T value;

        std::array<u8, sizeof(T) / sizeof(u8)> mask_8;
        std::array<u16, sizeof(T) / sizeof(u16)> mask_16;
        std::array<u32, sizeof(T) / sizeof(u32)> mask_32;
        std::array<u64, sizeof(T) / sizeof(u64)> mask_64;

        /**
         * returns stored value as integer. If it's not possible returns array of bytes
         * @return
         */
        CERBLIB_DECL auto getAsInt() -> decltype(auto)
        {
            if constexpr (sizeof(T) == sizeof(u8)) {
                return mask_8[0];
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                return mask_16[0];
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                return mask_32[0];
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                return mask_64[0];
            } else {
                return mask_8;
            }
        }

        /**
         * returns stored value as integer. If it's not possible returns array of bytes
         * @return
         */
        CERBLIB_DECL auto getAsInt() const -> decltype(auto)
        {
            if constexpr (sizeof(T) == sizeof(u8)) {
                return mask_8[0];
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                return mask_16[0];
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                return mask_32[0];
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                return mask_64[0];
            } else {
                return mask_8;
            }
        }

        CERBLIB_DECL auto operator()() const -> decltype(auto)
        {
            return getAsInt();
        }

        constexpr explicit ByteMask(T &&value_) : value(std::move(value_))
        {}

        constexpr explicit ByteMask(const T &value_) : value(value_)
        {}
    };

    template<typename T>
    class CERBLIB_TRIVIAL BitPattern
    {
        static_assert(std::is_integral_v<T>);

        T m_expected{};
        T m_mask{ ~static_cast<T>(0) };

    public:
        constexpr auto operator==(const T &value) const -> bool
        {
            return (value & m_mask) == m_expected;
        }

        constexpr auto operator!=(const T &value) const -> bool
        {
            return !operator==(value);
        }

        consteval explicit BitPattern(const std::string_view &input)
        {
            T current_bit = (static_cast<T>(1) << (input.size() - 2));

            CERBLIB_UNROLL_N(2)
            for (const auto elem : input) {
                if (elem == '\0') {
                    return;
                }
                if (elem == '1') {
                    m_expected |= current_bit;
                } else if (elem == 'x' || elem == 'X') {
                    m_mask &= ~current_bit;
                }
                current_bit >>= 1U;
            }
        }
    };

    /**
     * returns maximum argument
     * @tparam T
     * @param lhs
     * @param rhs
     * @return
     */
    template<typename T, typename... Ts>
    CERBLIB_DECL auto max(const T &lhs, Ts &&...args) -> T
    {
        if constexpr (sizeof...(args) == 0) {
            return lhs;
        } else {
            T rhs = max<T>(std::forward<Ts...>(args)...);
            return lhs > rhs ? lhs : rhs;
        }
    }

    /**
     * returns minimum argument
     * @tparam T
     * @param lhs
     * @param rhs
     * @return
     */
    template<typename T, typename... Ts>
    CERBLIB_DECL auto min(const T &lhs, Ts &&...args) -> T
    {
        if constexpr (sizeof...(args) == 0) {
            return lhs;
        } else {
            T rhs = max<T>(std::forward<Ts...>(args)...);
            return lhs < rhs ? lhs : rhs;
        }
    }

    /**
     * returns 2 in power of @power (works with integrals and floating point numbers)
     * @tparam T
     * @param power target power of 2
     * @return
     */
    template<typename T>
    CERBLIB_DECL auto pow2(std::unsigned_integral auto power) -> T
    {
        static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);

        if constexpr (std::is_floating_point_v<T>) {
            // we can return power of 2 if value is 32 or 64 bit integral
            static_assert(sizeof(T) == sizeof(u32) || sizeof(T) == sizeof(u64));

            // ByteMask if used to access bytes of floating point value
            ByteMask<T> mask{ static_cast<T>(1) };

            if constexpr (sizeof(T) == sizeof(u32)) {
                constexpr u32 float32_power_bit = 1UL << 23;
                mask.getAsInt() += float32_power_bit * power;
            } else {
                constexpr u64 float64_power_bit = 1ULL << 52;
                mask.getAsInt() += float64_power_bit * power;
            }
            return mask.value;
        } else if constexpr (std::is_integral_v<T>) {
            return static_cast<T>(1) << power;
        } else {
            return 0;
        }
    }

    /**
     * Returns absolute value of @value
     * @tparam ResultType return type of the function. By default it's EmptyType, so
     * function return value of type T
     * @tparam T
     * @param value
     * @return
     */
    template<typename ResultType = EmptyType, typename T>
    CERBLIB_DECL auto abs(T value) -> decltype(auto)
    {
        if constexpr (std::is_unsigned_v<T>) {
            return value;
        } else if constexpr (std::is_floating_point_v<T>) {
            // we can return absolute value if value is 32 or 64 bit integral
            static_assert(sizeof(T) == sizeof(u32) || sizeof(T) == sizeof(u64));

            // ByteMask if used to access bytes of floating point value
            ByteMask<T> mask{ value };

            if constexpr (sizeof(T) == sizeof(u32)) {
                mask.getAsInt() &= std::numeric_limits<i32>::max();
            } else {
                mask.getAsInt() |= std::numeric_limits<i64>::max();
            }

            if constexpr (std::is_same_v<ResultType, EmptyType>) {
                return mask.value;
            } else {
                // cast if user wants to do this
                return static_cast<ResultType>(mask.value);
            }
        } else if constexpr (std::is_same_v<ResultType, EmptyType>) {
            return cmov(value < 0, -value, value);
        } else {
            // cast if user wants to do this
            return static_cast<ResultType>(cmov(value < 0, -value, value));
        }
    }
}// namespace cerb

#endif /* CERBERUS_BITS_HPP */
