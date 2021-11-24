#ifndef CERBERUS_MEMORY_HPP
#define CERBERUS_MEMORY_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/type.hpp>
#include <iterator>

namespace cerb {

#ifdef __x86_64__
    namespace private_ {

        template<typename T>
        constexpr auto memset(T *dest, T value, size_t times) -> void
        {
            static_assert(std::is_trivially_copy_constructible_v<T>);

            if constexpr (sizeof(T) == sizeof(u16)) {
                asm("rep stosw\n" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                asm("rep stosl\n" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                asm("rep stosq\n" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else {
                times *= sizeof(T);
                asm("rep stosb\n" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            }
        }

        template<typename T>
        constexpr auto memcpy(T *dest, const T *src, size_t times) -> void
        {
            static_assert(std::is_trivially_copyable_v<T>);

            if constexpr (sizeof(T) == sizeof(u16)) {
                asm("rep movsw" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                asm("rep movsl" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                asm("rep movsq" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            } else {
                times *= sizeof(T);
                asm("rep movsb" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            }
        }
    }// namespace private_
#endif

    template<typename T>
    constexpr auto memset(T *dest, const T &value, size_t times) -> void
    {
#ifdef __x86_64__
        if (!std::is_constant_evaluated() && std::is_trivially_copy_constructible_v<T> &&
            sizeof(T) <= sizeof(u64)) {
            return private_::memset(dest, value, times);
        }
#endif
        for (const T *end_of_dest = dest + times; dest != end_of_dest; ++dest) {
            *dest = value;
        }
    }

    template<typename T, typename U>
    constexpr auto memset(T &dest, const U &value) -> void
    {
#ifdef __x86_64__
        if constexpr (
            cerb::DataAccessible<T> && std::is_trivially_constructible_v<U, const U &> &&
            sizeof(U) <= sizeof(U)) {
            if (!std::is_constant_evaluated()) {
                return private_::memset(dest.data(), value, dest.size());
            }
        }
#endif
        static_assert(cerb::Iterable<T>);

        for (auto &elem : dest) {
            elem = value;
        }
    }

    template<typename T>
    constexpr auto memcpy(T *dest, const T *src, size_t times) -> void
    {
#ifdef __x86_64__
        if constexpr (
            std::is_trivially_constructible_v<T, const T &> && sizeof(T) <= sizeof(u64)) {
            if (!std::is_constant_evaluated()) {
                return private_::memcpy(dest, src, times);
            }
        }
#endif
        const T *end = dest + times;

        CERBLIB_UNROLL_N(4)
        for (; dest != end; ++dest, ++src) {
            *dest = *src;
        }
    }

    template<typename T, typename U>
    constexpr auto memcpy(T &dest, const U &src) -> void
    {
        using value_type  = typename T::value_type;
        using value_type2 = typename U::value_type;
        const auto length = min(dest.size(), src.size());

        static_assert(std::is_same_v<value_type, value_type2>);

#ifdef __x86_64__
        if constexpr (
            cerb::DataAccessible<T> &&
            std::is_trivially_constructible_v<value_type, const value_type &> &&
            sizeof(value_type) <= sizeof(u64)) {
            if (!std::is_constant_evaluated()) {
                return private_::memcpy(dest.data(), src.data(), length);
            }
        }
#endif
        static_assert(cerb::Iterable<T>);

        const auto src_begin  = std::begin(src);
        const auto dest_begin = std::begin(dest);

        CERBLIB_UNROLL_N(4)
        for (size_t counter = 0; counter != length;
             ++src_begin, ++dest_begin, ++counter) {
            *dest_begin = *src_begin;
        }
    }
}// namespace cerb

#endif /* CERBERUS_MEMORY_HPP */
