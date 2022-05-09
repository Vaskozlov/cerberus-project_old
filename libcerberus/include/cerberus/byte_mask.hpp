#ifndef CERBERUS_BYTE_MASK_HPP
#define CERBERUS_BYTE_MASK_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/type.hpp>

namespace cerb
{
    template<typename T>
    union CERBLIB_TRIVIAL ByteMask {
        CERBLIB_DECL auto getAsInt() -> decltype(auto)
        {
            static_assert(CanBeStoredAsIntegral<T>);

            if constexpr (sizeof(T) == sizeof(u8)) {
                return mask_8[0];
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                return mask_16[0];
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                return mask_32[0];
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                return mask_64[0];
            } else if constexpr (sizeof(T) == sizeof(size_t)) {
                return mask_size_t[0];
            }
        }

        CERBLIB_DECL auto getAsInt() const -> decltype(auto)
        {
            static_assert(CanBeStoredAsIntegral<T>);

            if constexpr (sizeof(T) == sizeof(u8)) {
                return mask_8[0];
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                return mask_16[0];
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                return mask_32[0];
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                return mask_64[0];
            } else if constexpr (sizeof(T) == sizeof(size_t)) {
                return mask_size_t[0];
            }
        }

        constexpr explicit ByteMask(T &&value_to_store) : value(std::move(value_to_store))
        {}

        constexpr explicit ByteMask(T const &value_to_store) : value(value_to_store)
        {}

        T value;

        std::array<u8, sizeof(T) / sizeof(u8)> mask_8;
        std::array<u16, sizeof(T) / sizeof(u16)> mask_16;
        std::array<u32, sizeof(T) / sizeof(u32)> mask_32;
        std::array<u64, sizeof(T) / sizeof(u64)> mask_64;       // NOLINT
        std::array<u64, sizeof(T) / sizeof(size_t)> mask_size_t;// NOLINT
    };
}// namespace cerb

#endif /*  CERBERUS_BYTE_MASK_HPP */
