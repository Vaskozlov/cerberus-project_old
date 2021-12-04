#ifndef CERBERUS_MEMORY_HPP
#define CERBERUS_MEMORY_HPP

#include <cerberus/bits.hpp>
#include <cerberus/type.hpp>
#include <iterator>

namespace cerb {
#if CERBLIB_AMD64
    namespace private_ {

        /**
         * Fills memory with value N times. Must not be called directly.
         * @tparam T
         * @param dest
         * @param value
         * @param times
         * @return
         */
        template<typename T>
        constexpr auto memset(T *dest, T value, size_t times) -> void
        {
            /* we need to make sure, that value is can be stored in register,
             * and it is trivially copiable
             */
            static_assert(
                CanBeStoredInIntegral<T> && std::is_trivially_copy_constructible_v<T>);

            if constexpr (sizeof(T) == sizeof(u8)) {
                asm("rep stosb" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                asm("rep stosw" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                asm("rep stosl" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                asm("rep stosq" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            }
        }

        /**
         * Copies bytes from src to dest. Must not be called directly.
         * @tparam T
         * @param dest
         * @param src
         * @param times
         * @return
         */
        template<typename T>
        constexpr auto memcpy(T *dest, const T *src, size_t times) -> void
        {
            // We need to make sure that T is trivially copiable
            static_assert(std::is_trivially_copyable_v<T>);

            /* we can copy type which is bigger than register, because we can copy it in
             * different parts*/
            if constexpr (sizeof(T) % sizeof(u64) == 0) {
                times *= sizeof(T) / sizeof(u64);
                asm("rep movsq" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            } else if constexpr (sizeof(T) % sizeof(u32) == 0) {
                times *= sizeof(T) / sizeof(u32);
                asm("rep movsl" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            } else if constexpr (sizeof(T) % sizeof(u16) == 0) {
                times *= sizeof(T) / sizeof(u16);
                asm("rep movsw" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            } else {
                times *= sizeof(T);
                asm("rep movsb" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            }
        }

        /**
         * Finds first occurrence of the value in the location. Must not be called
         * directly.
         * @tparam T
         * @param location
         * @param value 64 bit representation of the value, which we need to find.
         * @return index of the value
         */
        template<typename T>
        constexpr auto find(const T *location, u64 value) -> size_t
        {
            /* we can find only types, which can be stored in register */
            static_assert(CanBeStoredInIntegral<T>);

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

        /**
         * Compares to arrays with the size of @length. Must not be called directly.
         * @tparam T
         * @param dest
         * @param src
         * @param length
         * @return true if they are equal, false otherwise
         */
        template<typename T>
        constexpr auto memcmp(const T *dest, const T *src, size_t length) -> bool
        {
            ++length;

            if constexpr (sizeof(T) % sizeof(u64) == 0) {
                length *= sizeof(T) / sizeof(u64);
                asm("repe cmpsq; shr $3, $2;"
                    : "+D"(dest), "+S"(src), "+c"(length)
                    :
                    : "memory");
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                length *= sizeof(T) / sizeof(u32);
                asm("repe cmpsl; shr $2, $2;"
                    : "+D"(dest), "+S"(src), "+c"(length)
                    :
                    : "memory");
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                length *= sizeof(T) / sizeof(u16);
                asm("repe cmpsw; shr $1, $2;"
                    : "+D"(dest), "+S"(src), "+c"(length)
                    :
                    : "memory");
            } else {
                length *= sizeof(T);
                asm("repe cmpsb;" : "+D"(dest), "+S"(src), "+c"(length) : : "memory");
            }

            return length == 0;
        }
    }// namespace private_
#endif

    /**
     * Sets memory region N times with given value.
     * @tparam T
     * @param dest
     * @param value
     * @param times
     * @return
     */
    template<typename T>
    constexpr auto memset(T *dest, const T &value, size_t times) -> void
    {
#if CERBLIB_AMD64
        if constexpr (
            std::is_trivially_copy_assignable_v<T> && CanBeStoredInIntegral<T>) {
            if (!std::is_constant_evaluated()) {
                return private_::memset(dest, value, times);
            }
        }
#endif
        for (const T *end_of_dest = dest + times; dest != end_of_dest; ++dest) {
            *dest = value;
        }
    }

    /**
     * Sets class/struct with given value.
     * @tparam T
     * @tparam U
     * @param dest
     * @param value
     * @return
     */
    template<Iterable T>
    constexpr auto memset(T &dest, const typename T::value_type &value) -> void
    {
#if CERBLIB_AMD64
        if constexpr (
            RawAccessible<T> && ClassValueFastCopiable<T> && CanBeStoredInIntegral<T>) {
            if (!std::is_constant_evaluated()) {
                return private_::memset(dest.data(), value, dest.size());
            }
        }
#endif
        for (typename T::value_type &elem : dest) {
            elem = value;
        }
    }

    template<typename T>
    constexpr auto memcpy(T *dest, const T *src, size_t times) -> void
    {
#if CERBLIB_AMD64
        if constexpr (std::is_trivially_copy_assignable_v<T>) {
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

    template<Iterable T, Iterable U>
    constexpr auto memcpy(T &dest, const U &src) -> void
    {
        using value_type  = typename T::value_type;
        using value_type2 = typename U::value_type;
        static_assert(std::is_same_v<value_type, value_type2>);

        const auto length = min(std::size(dest), std::size(src));

#if CERBLIB_AMD64
        if constexpr (
            RawAccessible<T> && RawAccessible<U> &&
            std::is_trivially_copy_assignable_v<value_type>) {
            if (!std::is_constant_evaluated()) {
                return private_::memcpy(dest.data(), src.data(), length);
            }
        }
#endif

        auto src_begin  = std::begin(src);
        auto dest_begin = std::begin(dest);

        CERBLIB_UNROLL_N(4)
        for (size_t i = 0; i != length; ++src_begin, ++dest_begin, ++i) {
            *dest_begin = *src_begin;
        }
    }

    template<typename T>
    constexpr auto find(const T *location, const T &value) -> size_t
    {
#if CERBLIB_AMD64
        if constexpr (CanBeStoredInIntegral<T>) {
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
        for (; *location != value; ++location) {
            // empty block
        }

        return static_cast<size_t>(static_cast<ptrdiff_t>(location - location_ptr));
    }

    template<CharType CharT>
    constexpr auto strlen(const CharT *str) -> size_t
    {
        return find(str, static_cast<CharT>(0));
    }

    template<Iterable T>
    constexpr auto equal(const T &lhs, const T &rhs) -> bool
    {
        auto length = min(std::size(lhs), std::size(rhs));

#if CERBLIB_AMD64
        if constexpr (RawAccessible<T> && CanBeStoredInIntegral<T>) {
            if (!std::is_constant_evaluated()) {
                return private_::memcmp(std::data(lhs), std::data(rhs), length);
            }
        }
#endif

        auto lhs_begin = lhs.cbegin();
        auto rhs_begin = rhs.cbegin();

        CERBLIB_UNROLL_N(4)
        for (size_t i = 0; i < length; ++i, ++lhs_begin, ++rhs_begin) {
            if (*lhs_begin != *rhs_begin) {
                return false;
            }
        }

        return true;
    }

    template<Iterable T>
    constexpr auto not_equal(const T &lhs, const T &rhs) -> bool
    {
        return !equal<T>(lhs, rhs);
    }
}// namespace cerb

#endif /* CERBERUS_MEMORY_HPP */
