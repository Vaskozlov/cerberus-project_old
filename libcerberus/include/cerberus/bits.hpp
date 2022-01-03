#ifndef CERBERUS_BITS_HPP
#define CERBERUS_BITS_HPP

#include <bit>
#include <cerberus/cerberus.hpp>
#include <cerberus/type.hpp>

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
         * returns stored value as integer.
         * @return
         */
        CERBLIB_DECL auto getAsInt() -> decltype(auto)
        {
            static_assert(CanBeStoredInIntegral<T>);

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
         * @return integral representation of stored value.
         */
        CERBLIB_DECL auto getAsInt() const -> decltype(auto)
        {
            static_assert(CanBeStoredInIntegral<T>);

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
         * @return integral representation of stored value.
         */
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

    template<typename T, typename... Ts>
    CERBLIB_DECL auto max(AutoCopyType<T> lhs, Ts &&...args) -> T
    {
        if constexpr (sizeof...(args) == 0) {
            return lhs;
        } else {
            T rhs = max<T>(std::forward<Ts>(args)...);
            return lhs > rhs ? lhs : rhs;
        }
    }

    template<typename T, typename... Ts>
    CERBLIB_DECL auto min(AutoCopyType<T> lhs, Ts &&...args) -> T
    {
        if constexpr (sizeof...(args) == 0) {
            return lhs;
        } else {
            T rhs = min<T>(std::forward<Ts>(args)...);
            return lhs < rhs ? lhs : rhs;
        }
    }

    template<std::unsigned_integral T, std::unsigned_integral PowerType>
    CERBLIB_DECL auto pow2(PowerType powerOf2) -> T
    {
        return static_cast<T>(1) << powerOf2;
    }

    template<std::floating_point T, std::unsigned_integral PowerType>
    CERBLIB_DECL auto pow2(PowerType powerOf2) -> T
    {
        static_assert(is_any_of_v<T, f32, f64>, "cerb::pow2 supports only floats and doubles.");

        ByteMask<T> mask{ static_cast<T>(1) };

        if constexpr (sizeof(T) == sizeof(f32)) {
            constexpr u32 f32_exponent = 1UL << 23;
            mask.getAsInt() += static_cast<u32>(f32_exponent * powerOf2);
        } else {
            constexpr u64 f64_exponent = 1ULL << 52;
            mask.getAsInt() += f64_exponent * powerOf2;
        }

        return mask.value;
    }

    template<std::integral T>
    CERBLIB_DECL auto abs(T value) -> T
    {
        if constexpr (std::unsigned_integral<T>) {
            return value;
        }
        return value < 0 ? -value : value;
    }

    CERBLIB_DECL auto abs(f32 value) -> f32
    {
        static_assert(sizeof(f32) == sizeof(u32));

        ByteMask mask{ value };
        mask.getAsInt() &= static_cast<u32>(std::numeric_limits<i32>::max());

        return mask.value;
    }

    CERBLIB_DECL auto abs(f64 value) -> f64
    {
        static_assert(sizeof(f64) == sizeof(u64), "cerb::abs supports only floats and doubles.");

        ByteMask mask{ value };
        mask.getAsInt() &= static_cast<u64>(std::numeric_limits<i64>::max());

        return mask.value;
    }

    template<std::equality_comparable T>
    CERBLIB_DECL auto safeEqual(T lhs, T rhs) -> bool
    {
        if constexpr (std::is_floating_point_v<T>) {
            return abs(lhs - rhs) <= std::numeric_limits<T>::epsilon();
        } else {
            return lhs == rhs;
        }
    }

    template<typename T>
    CERBLIB_DECL auto safeNotEqual(T lhs, T rhs) -> bool
    {
        if constexpr (std::is_floating_point_v<T>) {
            return abs(lhs - rhs) > std::numeric_limits<T>::epsilon();
        } else {
            return lhs != rhs;
        }
    }

    /**
     * Trunks integer
     * @tparam PowerOf2
     * @tparam T integer type
     * @param number
     * @return trunked integer
     */
    template<usize PowerOf2, std::integral T>
    CERBLIB_DECL auto trunk(T number) -> T
    {
        return number & ~(pow2<T>(PowerOf2) - 1);
    }

    /**
     * Ceils integer to power of 2
     * @tparam PowerOf2
     * @tparam T integer type
     * @param number
     * @return ceiled integer
     */
    template<usize PowerOf2, std::integral T>
    CERBLIB_DECL auto ceil(T number) -> T
    {
        return number + (pow2<T>(PowerOf2) - number % pow2<T>(PowerOf2));
    }

    /**
     * Align integer to power of 2
     * @tparam PowerOf2
     * @tparam T
     * @param value
     * @return
     */
    template<u64 PowerOf2, std::integral T>
    CERBLIB_DECL auto align(T value) -> T
    {
        const bool need2Align = value % pow2<T>(PowerOf2) == 0;
        return need2Align ? value : ceil<PowerOf2, T>(value);
    }

#ifdef _MSC_VER
    namespace private_ {
        template<unsigned BitValue, std::unsigned_integral T>
        CERBLIB_DECL auto bitScanForward(T value) -> usize
        {
            static_assert(BitValue == 0 || BitValue == 1);

            if constexpr (BitValue == 0) {
                value = ~value;
            }

            usize bit_index = 0;
            constexpr T mask = 0b1;

            CERBLIB_UNROLL_N(2)
            for (; value > 0; ++bit_index) {
                if ((value & mask) == mask) {
                    return bit_index;
                }
                value >>= 1;
            }

            return bitsizeof(usize);
        }

        template<unsigned BitValue, std::unsigned_integral T>
        CERBLIB_DECL auto bitScanReverse(T value) -> usize
        {
            static_assert(std::is_unsigned_v<T>);
            static_assert(BitValue == 0 || BitValue == 1);

            if constexpr (BitValue == 0) {
                value = ~value;
            }

            usize bit_index = bitsizeof(T) - 1;
            constexpr T mask = static_cast<T>(1) << (bitsizeof(T) - 1);

            CERBLIB_UNROLL_N(2)
            for (; value > 0; --bit_index) {
                if ((value & mask) == mask) {
                    return bit_index;
                }
                value <<= 1;
            }

            return bitsizeof(usize);
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
    template<unsigned BitValue, std::unsigned_integral T>
    CERBLIB_DECL auto bitScanForward(T value) -> usize
    {
        static_assert(BitValue == 0 || BitValue == 1);

        if constexpr (BitValue == 0) {
            value = ~value;
        }
        if (value == 0) {
            return bitsizeof(usize);
        }

#ifdef _MSC_VER
        if (std::is_constant_evaluated()) {
            return private_::bitScanForward<BitValue>(value);
        }

        unsigned long bit_index;

#    if defined(_WIN32) && !defined(_WIN64)
        _BitScanForward(&bit_index, static_cast<usize>(value));
#    else
        _BitScanForward64(&bit_index, static_cast<usize>(value));
#    endif

        return static_cast<usize>(bit_index);
#else
        return static_cast<usize>(__builtin_ctzl(static_cast<usize>(value)));
#endif
    }

    /**
     * Finds position of target bit in the number (left to right order)
     * @tparam BitValue target bit. Must be 0 or 1.
     * @tparam T unsigned integer
     * @param value, where we need to find the position of target bit
     * @return
     */
    template<unsigned BitValue, std::unsigned_integral T>
    CERBLIB_DECL auto bitScanReverse(T value) -> usize
    {
        static_assert(BitValue == 0 || BitValue == 1);

        if constexpr (BitValue == 0) {
            value = ~value;
        }
        if (value == 0) {
            return bitsizeof(usize);
        }

#ifdef _MSC_VER
        if (std::is_constant_evaluated()) {
            return private_::bitScanReverse<BitValue>(value);
        }

        unsigned long bit_index;

#    if defined(_WIN32) && !defined(_WIN64)
        _BitScanReverse(&bit_index, static_cast<usize>(value));
#    else
        _BitScanReverse64(&bit_index, static_cast<usize>(value));
#    endif

        return bit_index;
#else
        return bitsizeof(unsigned long) - 1UL -
               static_cast<usize>(__builtin_clzl(static_cast<usize>(value)));
#endif
    }

    CERBLIB_DECL auto log2(std::unsigned_integral auto number) -> usize
    {
        return bitScanForward<1>(number);
    }

    CERBLIB_DECL auto log2(f32 number) -> isize
    {
        static_assert(sizeof(f32) == sizeof(u32), "cerb::log2 supports floats and doubles");

        if (number <= 0.0f) {
            return -1;
        }

        const u32 mask = std::bit_cast<u32>(number);
        constexpr i32 f32_exponent_bit = 23;
        constexpr i32 f32_exponent_for_zero_power = 0x7fU;
        constexpr u32 f32_exponent_mask = 0xFF80'0000;
        return static_cast<isize>((mask & f32_exponent_mask) >> f32_exponent_bit) -
               f32_exponent_for_zero_power;
    }

    CERBLIB_DECL auto log2(f64 number) -> isize
    {
        const u64 mask = std::bit_cast<u64>(number);
        constexpr i64 f64_exponent_bit = 52;
        constexpr i64 f64_exponent_for_zero_power = 0x3ffU;
        constexpr u64 f64_exponent_mask = 0xFFF0'0000'0000'0000;
        return static_cast<isize>((mask & f64_exponent_mask) >> f64_exponent_bit) -
               f64_exponent_for_zero_power;
    }
}// namespace cerb

#endif /* CERBERUS_BITS_HPP */
