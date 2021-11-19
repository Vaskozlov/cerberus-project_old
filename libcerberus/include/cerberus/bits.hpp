#ifndef CERBERUS_BITS_HPP
#define CERBERUS_BITS_HPP

#include <bit>
#include <cerberus/cerberus.hpp>

namespace cerb {
    enum struct AlignRule
    {
        DEFAULT,
        TRUNK,
        CEIL
    };

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

        CERBLIB_DECL static auto canBeGetAsInt() -> bool
        {
            switch (sizeof(T)) {
            case sizeof(u8):
            case sizeof(u16):
            case sizeof(u32):
            case sizeof(u64):
                return true;
            default:
                return false;
            }
        }

        /**
         * returns stored value as integer.
         * @return
         */
        CERBLIB_DECL auto getAsInt() -> decltype(auto)
        {
            static_assert(canBeGetAsInt());

            if constexpr (sizeof(T) == sizeof(u8)) {
                return mask_8[0];
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                return mask_16[0];
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                return mask_32[0];
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                return mask_64[0];
            }
        }

        /**
         * returns stored value as integer.
         * @return
         */
        CERBLIB_DECL auto getAsInt() const -> decltype(auto)
        {
            static_assert(canBeGetAsInt());

            if constexpr (sizeof(T) == sizeof(u8)) {
                return mask_8[0];
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                return mask_16[0];
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                return mask_32[0];
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                return mask_64[0];
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
            T rhs = max<T>(args...);
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
            T rhs = min<T>(args...);
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
                mask.getAsInt() += static_cast<u32>(float32_power_bit * power);
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
                mask.getAsInt() &= static_cast<u32>(std::numeric_limits<i32>::max());
            } else {
                mask.getAsInt() &= static_cast<u64>(std::numeric_limits<i64>::max());
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

    /**
     * Safely compare floating point numbers
     * @tparam T
     * @param lhs
     * @param rhs
     * @return
     */
    template<typename T>
    CERBLIB_DECL auto equal(T lhs, T rhs) -> bool
    {
        if constexpr (std::is_floating_point_v<T>) {
            return abs(lhs - rhs) <= std::numeric_limits<T>::epsilon();
        } else {
            return lhs == rhs;
        }
    }

    /**
     * Safely compare floating point numbers
     * @tparam T
     * @param lhs
     * @param rhs
     * @return
     */
    template<typename T>
    CERBLIB_DECL auto not_equal(T lhs, T rhs) -> bool
    {
        if constexpr (std::is_floating_point_v<T>) {
            return abs(lhs - rhs) > std::numeric_limits<T>::epsilon();
        } else {
            return lhs != rhs;
        }
    }

    template<u64 power, AlignRule rule = AlignRule::DEFAULT, typename T>
    CERBLIB_DECL auto align(T value) -> T
    {
        static_assert(std::is_integral_v<T>);

        if constexpr (rule == AlignRule::TRUNK) {
            // if we need to trunk the value, we just set bits under power to 0
            return value & ~(pow2<T>(power) - 1);
        } else if constexpr (rule == AlignRule::CEIL) {
            // if we need to ceil the number we add something to it, so it can become
            // aligned
            return value + (pow2<T>(power) - value % pow2<T>(power));
        } else if constexpr (rule == AlignRule::DEFAULT) {
            // in normal mode we check that the number is aligned, and if it is not, we
            // ceil it
            return value % pow2<T>(power) == 0 ? value
                                               : align<power, AlignRule::CEIL>(value);
        }
    }

#ifdef _MSC_VER
    namespace private_ {
        /**
         * Finds position of target bit in the number (right to left order)
         * @tparam BitValue target bit. Must be 0 or 1.
         * @tparam T unsigned integer
         * @param value, where we need to find the position of target bit
         * @return
         */
        template<unsigned BitValue, typename T>
        CERBLIB_DECL auto bitScanForward(T value) -> u64
        {
            static_assert(std::is_unsigned_v<T>);
            static_assert(BitValue == 0 || BitValue == 1);

            if constexpr (BitValue == 0) {
                value = ~value;
            }

            u64 bit_index    = 0;
            constexpr T mask = 0b1;

            CERBLIB_UNROLL_N(2)
            for (; value > 0; ++bit_index) {
                if ((value & mask) == mask) {
                    return bit_index;
                }
                value >>= 1;
            }

            return bitsizeof(uintmax_t);
        }

        /**
         * Finds position of target bit in the number (left to right order)
         * @tparam BitValue target bit. Must be 0 or 1.
         * @tparam T unsigned integer
         * @param value, where we need to find the position of target bit
         * @return
         */
        template<unsigned BitValue, typename T>
        CERBLIB_DECL auto bitScanReverse(T value) -> u64
        {
            static_assert(std::is_unsigned_v<T>);
            static_assert(BitValue == 0 || BitValue == 1);

            if constexpr (BitValue == 0) {
                value = ~value;
            }

            u64 bit_index    = bitsizeof(T) - 1;
            constexpr T mask = static_cast<T>(1) << (bitsizeof(T) - 1);

            CERBLIB_UNROLL_N(2)
            for (; value > 0; --bit_index) {
                if ((value & mask) == mask) {
                    return bit_index;
                }
                value <<= 1;
            }

            return bitsizeof(uintmax_t);
        }
    }  // namespace private_
#endif /* _MSC_VER */

    /**
     * Finds position of target bit in the number (right to left order)
     * @tparam BitValue target bit. Must be 0 or 1.
     * @tparam T unsigned integer
     * @param value, where we need to find the position of target bit
     * @return
     */
    template<unsigned BitValue, typename T>
    CERBLIB_DECL auto bitScanForward(T value) -> u64
    {
        static_assert(std::is_unsigned_v<T>);
        static_assert(BitValue == 0 || BitValue == 1);

        if constexpr (BitValue == 0) {
            value = ~value;
        }
        if (value == 0) {
            return bitsizeof(uintmax_t);
        }

#ifdef _MSC_VER
        if (std::is_constant_evaluated()) {
            return private_::bitScanForward<BitValue>(value);
        }

        unsigned long bit_index;
        _BitScanForward64(&bit_index, static_cast<u64>(value));

        return static_cast<u64>(bit_index);
#else
        return static_cast<u64>(__builtin_ctzl(static_cast<u64>(value)));
#endif
    }

    /**
     * Finds position of target bit in the number (left to right order)
     * @tparam BitValue target bit. Must be 0 or 1.
     * @tparam T unsigned integer
     * @param value, where we need to find the position of target bit
     * @return
     */
    template<unsigned BitValue, typename T>
    CERBLIB_DECL auto bitScanReverse(T value) -> u64
    {
        static_assert(std::is_unsigned_v<T>);
        static_assert(BitValue == 0 || BitValue == 1);

        if constexpr (BitValue == 0) {
            value = ~value;
        }
        if (value == 0) {
            return bitsizeof(uintmax_t);
        }

#ifdef _MSC_VER
        if (std::is_constant_evaluated()) {
            return private_::bitScanForward<BitValue>(value);
        }

        unsigned long bit_index;
        _BitScanReverse64(&bit_index, static_cast<u64>(value));

        return static_cast<u64>(bitsizeof(unsigned long) - 1ULL - bit_index);
#else
        return bitsizeof(unsigned long) - 1UL - static_cast<u64>(__builtin_clzl(static_cast<u64>(value)));
#endif
    }

    /**
     * Finds log2 for integer and floating point numbers (returning value is trunked to
     * integer).
     * @tparam T
     * @param value
     * @return
     */
    template<typename T>
    CERBLIB_DECL auto log2(T value) -> decltype(auto)
    {
        static_assert(
            ((std::is_integral_v<T> && std::is_unsigned_v<T>) ||
             std::is_floating_point_v<T>));

        if constexpr (std::is_integral_v<T>) {
            return bitScanForward<1>(value);
        } else {
            static_assert(sizeof(T) <= sizeof(u64));
            if (value <= static_cast<T>(0.0)) {
                return static_cast<i64>(-1);
            }

            if constexpr (sizeof(T) == sizeof(u32)) {
                const u32 mask = std::bit_cast<u32>(value);

                // float32_zero_power - is 2^0 for 32 bit floating point number
                constexpr i32 float32_zero_power = 0x7fU;
                constexpr u32 float32_power_bit  = 23;
                constexpr u32 float32_power_mask = 0xFF80'0000;
                return static_cast<i64>(
                           (mask & float32_power_mask) >> float32_power_bit) -
                       float32_zero_power;
            } else {
                const u64 mask = std::bit_cast<u32>(value);

                // float64_zero_power - is 2^0 for 64 bit floating point number
                constexpr i64 float64_zero_power = 0x3ffU;
                constexpr u64 float64_power_bit  = 52;
                constexpr u64 float64_power_mask = 0xFFF0'0000'0000'0000;
                return static_cast<i64>(
                           (mask & float64_power_mask) >> float64_power_bit) -
                       float64_zero_power;
            }
        }
    }
}// namespace cerb

#endif /* CERBERUS_BITS_HPP */
