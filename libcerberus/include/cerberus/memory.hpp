#ifndef CERBERUS_MEMORY_HPP
#define CERBERUS_MEMORY_HPP

#include <cerberus/bits.hpp>
#include <cerberus/type.hpp>
#include <iterator>

namespace cerb {
#ifdef __x86_64__
    namespace private_ {

        template<typename T>
        constexpr auto memset(T *dest, T value, size_t times) -> void
        {
            static_assert(
                sizeof(T) <= sizeof(u64) && std::is_trivially_copy_constructible_v<T>);

            if constexpr (sizeof(T) == sizeof(u8)) {
                asm("rep stosb\n" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                asm("rep stosw\n" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                asm("rep stosl\n" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                asm("rep stosq\n" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
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

        template<typename T>
        constexpr auto find(const T *location, u64 value) -> size_t
        {
            static_assert(sizeof(T) <= sizeof(u64));

            u64 rax      = value;
            size_t limit = std::numeric_limits<u32>::max();

            if constexpr (sizeof(T) == sizeof(u8)) {
                asm("repnz scasb; sub %1, %0; mov %0, %3; dec %3;"
                    : "+D"(location), "+S"(location), "+c"(limit), "+a"(rax)
                    :
                    : "memory");
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                asm("repnz scasw; sub %1, %0; mov %0, %3; shr $1, %3; dec %3;"
                    : "+D"(location), "+S"(location), "+c"(limit), "+a"(rax)
                    :
                    : "memory");
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                asm("repnz scasl; sub %1, %0; mov %0, %3; shr $2, %3; dec %3;"
                    : "+D"(location), "+S"(location), "+c"(limit), "+a"(rax)
                    :
                    : "memory");
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                asm("repnz scasq; sub %1, %0; mov %0, %3; shr $3, %3; dec %3;"
                    : "+D"(location), "+S"(location), "+c"(limit), "+a"(rax)
                    :
                    : "memory");
            }

            return rax;
        }
    }// namespace private_
#endif

    template<typename T>
    constexpr auto memset(T *dest, const T &value, size_t times) -> void
    {
#ifdef __x86_64__
        if constexpr (FastCopiable<T>) {
            if (!std::is_constant_evaluated()) {
                return private_::memset(dest, value, times);
            }
        }
#endif
        for (const T *end_of_dest = dest + times; dest != end_of_dest; ++dest) {
            *dest = value;
        }
    }

    template<typename T, typename U>
    constexpr auto memset(T &dest, const U &value) -> void
    {
        static_assert(std::is_same_v<U, typename T::value_type>);

#ifdef __x86_64__
        if constexpr (ClassValueFastCopiable<T>) {
            if (!std::is_constant_evaluated()) {
                return private_::memset(dest.data(), value, dest.size());
            }
        }
#endif
        static_assert(cerb::RawAccessible<T>);

        for (auto &elem : dest) {
            elem = value;
        }
    }

    template<typename T>
    constexpr auto memcpy(T *dest, const T *src, size_t times) -> void
    {
#ifdef __x86_64__
        if constexpr (FastCopiable<T>) {
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
        if constexpr (ClassValueFastCopiable<T>) {
            if (!std::is_constant_evaluated()) {
                return private_::memcpy(dest.data(), src.data(), length);
            }
        }
#endif
        static_assert(cerb::RawAccessible<T>);

        auto src_begin  = std::begin(src);
        auto dest_begin = std::begin(dest);

        CERBLIB_UNROLL_N(4)
        for (size_t counter = 0; counter != length;
             ++src_begin, ++dest_begin, ++counter) {
            *dest_begin = *src_begin;
        }
    }

    template<typename T>
    constexpr auto find(const T *location, const T &value) -> size_t
    {
#ifdef __x86_64__
        if constexpr (sizeof(T) <= sizeof(u64)) {
            u64 value2find;

            if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>) {
                value2find = value;
            } else {
                ByteMask<T> mask{ value };
                value2find = mask();
            }
            return private_::find(location, value2find);
        }
#endif
        const T *location_ptr = location;

        CERBLIB_UNROLL_N(4)
        for (; *location != value; ++location) {}

        return static_cast<size_t>(static_cast<ptrdiff_t>(location - location_ptr));
    }
}// namespace cerb

#endif /* CERBERUS_MEMORY_HPP */
