#ifndef CERBERUS_NUMBER_HPP
#define CERBERUS_NUMBER_HPP

#include <cerberus/bit.hpp>
#include <cerberus/byte_mask.hpp>

namespace cerb
{
    template<typename T, typename... Ts>
    CERBLIB_DECL auto max(T arg, Ts... args) -> T
    {
        if constexpr (sizeof...(args) == 0) {
            return arg;
        } else {
            T rhs = max<T>(args...);

#ifdef __clang__
            if constexpr (std::integral<T>) {
                std::array temporary = { arg, rhs };
                return temporary[arg < rhs];
            }
#endif
            return arg > rhs ? arg : rhs;
        }
    }

    template<typename T, typename... Ts>
    CERBLIB_DECL auto min(T arg, Ts &&...args) -> T
    {
        if constexpr (sizeof...(args) == 0) {
            return arg;
        } else {
            T rhs = min<T>(args...);

#ifdef __clang__
            if constexpr (std::integral<T>) {
                std::array temporary = { arg, rhs };
                return temporary[arg > rhs];
            }
#endif
            return arg < rhs ? arg : rhs;
        }
    }

    template<std::unsigned_integral T, std::unsigned_integral PowerType>
    CERBLIB_DECL auto pow2(PowerType power_of_2) -> T
    {
        return static_cast<T>(1) << power_of_2;
    }

    template<std::floating_point T, std::unsigned_integral PowerType>
    CERBLIB_DECL auto pow2(PowerType power_of_2) -> T
    {
        static_assert(IsAnyOfV<T, f32, f64>, "cerb::pow2 supports only floats and doubles.");

        ByteMask mask{ static_cast<T>(1) };

        if constexpr (sizeof(T) == sizeof(f32)) {
            constexpr u32 f32_exponent = 1UL << 23;
            mask.getAsInt() += static_cast<u32>(f32_exponent * power_of_2);
        } else {
            constexpr u64 f64_exponent = 1ULL << 52;
            mask.getAsInt() += f64_exponent * power_of_2;
        }

        return mask.value;
    }

    template<std::integral T>
    CERBLIB_DECL auto abs(T value) -> T
    {
        if constexpr (std::unsigned_integral<T>) {
            return value;
        }
        return cmov(value < 0, -value, value);
    }

    CERBLIB_DECL auto abs(f32 value) -> f32
    {
        static_assert(sizeof(f32) == sizeof(u32));

        ByteMask mask{ value };

        if (std::is_constant_evaluated()) {
            mask.value = -mask.value;
        } else {
            mask.getAsInt() &= static_cast<u32>(std::numeric_limits<i32>::max());
        }

        return mask.value;
    }

    CERBLIB_DECL auto abs(f64 value) -> f64
    {
        static_assert(sizeof(f64) == sizeof(u64), "cerb::abs supports only floats and doubles.");

        ByteMask mask{ value };

        if (std::is_constant_evaluated()) {
            mask.value = -mask.value;
        } else {
            mask.getAsInt() &= static_cast<u64>(std::numeric_limits<i64>::max());
        }

        return mask.value;
    }

    template<typename T>
    CERBLIB_DECL auto convert2UnsignedInt(T number) -> decltype(auto)
    {
        static_assert(std::integral<T> || std::is_enum_v<T>);

        if constexpr (std::is_unsigned_v<T>) {
            return number;
        } else if constexpr (sizeof(T) == sizeof(u8)) {
            return std::bit_cast<u8>(number);
        } else if constexpr (sizeof(T) == sizeof(u16)) {
            return std::bit_cast<u16>(number);
        } else if constexpr (sizeof(T) == sizeof(u32)) {
            return std::bit_cast<u32>(number);
        } else if constexpr (sizeof(T) == sizeof(u64)) {
            return std::bit_cast<u64>(number);
        } else if constexpr (sizeof(T) == sizeof(size_t)) {
            return std::bit_cast<size_t>(number);
        }
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

    template<std::unsigned_integral UInt>
    CERBLIB_DECL auto log2(UInt number) -> size_t
    {
        return bit::scanForward<1>(number);
    }

    CERBLIB_DECL auto log2(f32 number) -> ssize_t
    {
        if (number <= 0.0f) {
            return -1;
        }

        u32 const mask = std::bit_cast<u32>(number);
        constexpr i32 f32_exponent_bit = 23;
        constexpr i32 f32_exponent_for_zero_power = 0x7fU;
        constexpr u32 f32_exponent_mask = 0xFF80'0000;
        return static_cast<ssize_t>((mask & f32_exponent_mask) >> f32_exponent_bit) -
               f32_exponent_for_zero_power;
    }

    CERBLIB_DECL auto log2(f64 number) -> ssize_t
    {
        if (number <= 0.0) {
            return -1;
        }

        u64 const mask = std::bit_cast<u64>(number);
        constexpr i64 f64_exponent_bit = 52;
        constexpr i64 f64_exponent_for_zero_power = 0x3ffU;
        constexpr u64 f64_exponent_mask = 0xFFF0'0000'0000'0000;
        return static_cast<ssize_t>((mask & f64_exponent_mask) >> f64_exponent_bit) -
               f64_exponent_for_zero_power;
    }
}// namespace cerb

#endif /* CERBERUS_NUMBER_HPP */
