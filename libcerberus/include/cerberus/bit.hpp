#ifndef CERBERUS_BIT_HPP
#define CERBERUS_BIT_HPP

#include <cerberus/byte_mask.hpp>

namespace cerb::bit
{
#ifdef _MSC_VER
    namespace msvc
    {
        template<std::unsigned_integral UInt>
        CERBLIB_DECL auto scanForwardCompileTime(UInt value) -> size_t
        {
            size_t bit_index = 0;
            constexpr UInt mask = 0b1;

            for (; value > 0; ++bit_index) {
                if ((value & mask) == mask) {
                    return bit_index;
                }
                value >>= 1;
            }

            return bitsizeof(size_t);
        }

        template<std::unsigned_integral UInt>
        CERBLIB_DECL auto scanForwardRuntime(UInt value) -> size_t
        {
            unsigned long bit_index;

#    if defined(_WIN32) && !defined(_WIN64)
            _BitScanForward(&bit_index, static_cast<size_t>(value));
#    else
            _BitScanForward64(&bit_index, static_cast<size_t>(value));
#    endif
            return static_cast<size_t>(bit_index);
        }

        template<std::unsigned_integral UInt>
        CERBLIB_DECL auto scanReverseCompileTime(UInt value) -> size_t
        {
            size_t bit_index = bitsizeof(UInt) - 1;
            constexpr UInt mask = static_cast<UInt>(1) << (bitsizeof(UInt) - 1);

            for (; value > 0; --bit_index) {
                if ((value & mask) == mask) {
                    return bit_index;
                }
                value <<= 1;
            }

            return bitsizeof(size_t);
        }

        template<std::unsigned_integral UInt>
        CERBLIB_DECL auto scanReverseRuntime(UInt value) -> size_t
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
        template<std::unsigned_integral UInt, std::unsigned_integral PowerType>
        CERBLIB_DECL auto pow2(PowerType power_of_2) -> UInt
        {
            return static_cast<UInt>(1) << power_of_2;
        }

        template<std::unsigned_integral UInt>
        CERBLIB_DECL auto callSystemScanForward(UInt value) -> size_t
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

        template<std::unsigned_integral UInt>
        CERBLIB_DECL auto callSystemScanReverse(UInt value) -> size_t
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

    template<size_t PowerOf2, std::integral Int>
    CERBLIB_DECL auto trunk(Int number) -> Int
    {
        using namespace private_;

        return number & ~(pow2<Int>(PowerOf2) - 1);
    }

    template<size_t PowerOf2, std::integral Int>
    CERBLIB_DECL auto ceil(Int number) -> Int
    {
        using namespace private_;

        return number + (pow2<Int>(PowerOf2) - number % pow2<Int>(PowerOf2));
    }

    template<u64 PowerOf2, std::integral Int>
    CERBLIB_DECL auto align(Int value) -> Int
    {
        using namespace private_;

        bool need_to_align = value % pow2<Int>(PowerOf2) == 0;
        return need_to_align ? value : ceil<PowerOf2, Int>(value);
    }

    template<unsigned BitValue, std::unsigned_integral UInt>
    CERBLIB_DECL auto scanForward(UInt value) -> size_t
    {
        using namespace private_;
        static_assert(isOneOf(BitValue, 0, 1));

        if constexpr (BitValue == 0) {
            value = ~value;
        }
        if (value == 0) {
            return bitsizeof(size_t);
        }

        return callSystemScanForward(value);
    }

    template<unsigned BitValue, std::unsigned_integral UInt>
    CERBLIB_DECL auto scanReverse(UInt value) -> size_t
    {
        using namespace private_;
        static_assert(isOneOf(BitValue, 0, 1));

        if constexpr (BitValue == 0) {
            value = ~value;
        }

        if (value == 0) {
            return bitsizeof(size_t);
        }

        return callSystemScanReverse(value);
    }
}// namespace cerb::bit

#endif /* CERBERUS_BIT_HPP */
