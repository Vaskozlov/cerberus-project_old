#ifndef CERBERUS_BIT_HPP
#define CERBERUS_BIT_HPP

#include <cerberus/byte_mask.hpp>

namespace cerb::bit
{
#ifdef _MSC_VER
    namespace msvc
    {
        template<std::unsigned_integral T>
        CERBLIB_DECL auto scanForwardCompileTime(T value) -> size_t
        {
            size_t bit_index = 0;
            constexpr T mask = 0b1;

            for (; value > 0; ++bit_index) {
                if ((value & mask) == mask) {
                    return bit_index;
                }
                value >>= 1;
            }

            return bitsizeof(size_t);
        }

        template<std::unsigned_integral T>
        CERBLIB_DECL auto scanForwardRuntime(T value) -> size_t
        {
            unsigned long bit_index;

#    if defined(_WIN32) && !defined(_WIN64)
            _BitScanForward(&bit_index, static_cast<size_t>(value));
#    else
            _BitScanForward64(&bit_index, static_cast<size_t>(value));
#    endif
            return static_cast<size_t>(bit_index);
        }

        template<std::unsigned_integral T>
        CERBLIB_DECL auto scanReverseCompileTime(T value) -> size_t
        {
            size_t bit_index = bitsizeof(T) - 1;
            constexpr T mask = static_cast<T>(1) << (bitsizeof(T) - 1);

            for (; value > 0; --bit_index) {
                if ((value & mask) == mask) {
                    return bit_index;
                }
                value <<= 1;
            }

            return bitsizeof(size_t);
        }

        template<std::unsigned_integral T>
        CERBLIB_DECL auto scanReverseRuntime(T value) -> size_t
        {
            unsigned long bit_index;

#    if defined(_WIN32) && !defined(_WIN64)
            _BitScanReverse(&bit_index, static_cast<size_t>(value));
#    else
            _BitScanReverse64(&bit_index, static_cast<size_t>(value));
#    endif
            return static_cast<size_t>(bit_index);
        }
    }  // namespace msvc
#endif /* _MSC_VER */

    namespace private_
    {
        template<std::unsigned_integral T, std::unsigned_integral PowerType>
        CERBLIB_DECL auto pow2(PowerType power_of_2) -> T
        {
            return static_cast<T>(1) << power_of_2;
        }

        template<std::unsigned_integral T>
        CERBLIB_DECL auto systemScanForward(T value) -> size_t
        {
#ifdef _MSC_VER
            if CERBLIB_COMPILE_TIME {
                return msvc::scanForwardCompileTime(value);
            } else {
                return msvc::scanForwardRuntime(value);
            }
#else
            return static_cast<size_t>(__builtin_ctzl(static_cast<size_t>(value)));
#endif
        }

        template<std::unsigned_integral T>
        CERBLIB_DECL auto systemScanReverse(T value) -> size_t
        {
#ifdef _MSC_VER
            if CERBLIB_COMPILE_TIME {
                return msvc::scanReverseCompileTime(value);
            } else {
                return msvc::scanReverseRuntime(value);
            }
#else
            return bitsizeof(size_t) - 1UL -
                   static_cast<size_t>(__builtin_clzl(static_cast<size_t>(value)));
#endif
        }
    }// namespace private_

    template<size_t PowerOf2, std::integral T>
    CERBLIB_DECL auto trunk(T number) -> T
    {
        return number & ~(private_::pow2<T>(PowerOf2) - 1);
    }

    template<size_t PowerOf2, std::integral T>
    CERBLIB_DECL auto ceil(T number) -> T
    {
        return number + (private_::pow2<T>(PowerOf2) - number % private_::pow2<T>(PowerOf2));
    }

    template<u64 PowerOf2, std::integral T>
    CERBLIB_DECL auto align(T value) -> T
    {
        bool const need_to_align = value % private_::pow2<T>(PowerOf2) == 0;
        return need_to_align ? value : ceil<PowerOf2, T>(value);
    }

    template<unsigned BitValue, std::unsigned_integral T>
    CERBLIB_DECL auto scanForward(T value) -> size_t
    {
        static_assert(BitValue == 0 || BitValue == 1);

        if constexpr (BitValue == 0) {
            value = ~value;
        }
        if (value == 0) {
            return bitsizeof(size_t);
        }

        return private_::systemScanForward(value);
    }

    template<unsigned BitValue, std::unsigned_integral T>
    CERBLIB_DECL auto scanReverse(T value) -> size_t
    {
        static_assert(BitValue == 0 || BitValue == 1);

        if constexpr (BitValue == 0) {
            value = ~value;
        }
        if (value == 0) {
            return bitsizeof(size_t);
        }

        return private_::systemScanReverse(value);
    }
}// namespace cerb::bit

#endif /* CERBERUS_BIT_HPP */
