#ifndef CERBERUS_NUMBER_HPP
#define CERBERUS_NUMBER_HPP

#include <cerberus/bit.hpp>
#include <cerberus/byte_mask.hpp>

namespace cerb
{
    constexpr i64 F32ExponentBit = 23;
    constexpr i64 F32ZeroExponent = 0x7fU;
    constexpr u64 F32ExponentMask = 0xFF80'0000;

    constexpr i64 F64ExponentBit = 52;
    constexpr i64 F64ZeroExponent = 0x3ffU;
    constexpr u64 F64ExponentMask = 0xFFF0'0000'0000'0000;

    template<typename T>
    CERBLIB_DECL auto asInt(T number) -> decltype(auto)
    {
        static_assert(std::integral<T> || std::is_enum_v<T> || std::is_floating_point_v<T>);

        if constexpr (std::is_unsigned_v<T>) {
            return number;
        } else if constexpr (sizeof(T) == sizeof(i8)) {
            return std::bit_cast<i8>(number);
        } else if constexpr (sizeof(T) == sizeof(i16)) {
            return std::bit_cast<i16>(number);
        } else if constexpr (sizeof(T) == sizeof(i32)) {
            return std::bit_cast<i32>(number);
        } else if constexpr (sizeof(T) == sizeof(i64)) {
            return std::bit_cast<i64>(number);
        } else if constexpr (sizeof(T) == sizeof(ssize_t)) {
            return std::bit_cast<ssize_t>(number);
        }
    }

    template<typename T>
    CERBLIB_DECL auto asUInt(T number) -> decltype(auto)
    {
        static_assert(std::integral<T> || std::is_enum_v<T> || std::is_floating_point_v<T>);

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

    template<std::floating_point T>
    CERBLIB_DECL auto getExponent(T number) -> ssize_t
    {
        static_assert(IsAnyOfV<T, f32, f64>);

        constexpr bool is_32bit_size = sizeof(T) == sizeof(u32);

        constexpr i64 exponent_bit = is_32bit_size ? F32ExponentBit : F64ExponentBit;
        constexpr i64 zero_exponent = is_32bit_size ? F32ZeroExponent : F64ZeroExponent;
        constexpr u64 exponent_mask = is_32bit_size ? F32ExponentMask : F64ExponentMask;

        auto mask = std::bit_cast<decltype(asUInt(number))>(number);
        auto exponent_of_mask = mask & exponent_mask;
        auto exponent_as_uint = exponent_of_mask >> exponent_bit;
        ssize_t exponent = static_cast<ssize_t>(exponent_as_uint) - zero_exponent;

        return exponent;
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

        constexpr u64 f32_exponent = 1ULL << F32ExponentBit;
        constexpr u64 f64_exponent = 1ULL << F64ExponentBit;
        constexpr u64 float_exponent = sizeof(T) == sizeof(u32) ? f32_exponent : f64_exponent;

        auto mask = asUInt(static_cast<T>(1));

        mask += power_of_2 * float_exponent;
        return std::bit_cast<T>(mask);
    }

    template<std::integral T>
    CERBLIB_DECL auto abs(T value) -> T
    {
        if constexpr (std::unsigned_integral<T>) {
            return value;
        }
        return cmov(value < 0, -value, value);
    }

    template<std::floating_point T>
    CERBLIB_DECL auto abs(T value) -> T
    {
        static_assert(IsAnyOfV<T, f32, f64>, "cerb::abs supports only f32 and f64.");

        auto mask = asInt(value);
        mask &= std::numeric_limits<decltype(mask)>::max();

        return std::bit_cast<T>(mask);
    }

    template<std::equality_comparable T>
    CERBLIB_DECL auto safeEqual(T lhs, T rhs) -> bool
    {
        if constexpr (std::floating_point<T>) {
            return abs(lhs - rhs) <= std::numeric_limits<T>::epsilon();
        } else {
            return lhs == rhs;
        }
    }

    template<typename T>
    CERBLIB_DECL auto safeNotEqual(T lhs, T rhs) -> bool
    {
        if constexpr (std::floating_point<T>) {
            return abs(lhs - rhs) > std::numeric_limits<T>::epsilon();
        } else {
            return lhs != rhs;
        }
    }

    template<std::unsigned_integral UInt>
    CERBLIB_DECL auto log2(UInt number) -> ssize_t
    {
        if (number == 0) {
            return -1;
        }

        return static_cast<ssize_t>(bit::scanForward<1>(number));
    }

    template<std::floating_point Float>
    CERBLIB_DECL auto log2(Float number) -> ssize_t
    {
        if (number <= static_cast<Float>(0.0)) {
            return -1;
        }

        return getExponent(number);
    }
}// namespace cerb

#endif /* CERBERUS_NUMBER_HPP */
