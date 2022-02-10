#ifndef CERBERUS_MEMORY_HPP
#define CERBERUS_MEMORY_HPP

#include <cerberus/bits.hpp>
#include <cerberus/type.hpp>
#include <iterator>

namespace cerb
{
#if CERBLIB_AMD64
    namespace amd64
    {
        template<typename T>
        constexpr auto fill(T *dest, T value, size_t times) -> void
        {
            /* we need to make sure, that value is can be stored in register,
             * and it is trivially copiable
             */
            static_assert(CanBeStoredInIntegral<T> && std::is_trivially_copy_constructible_v<T>);

            if constexpr (sizeof(T) == sizeof(u8)) {
                asm volatile("rep stosb" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                asm volatile("rep stosw" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                asm volatile("rep stosl" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                asm volatile("rep stosq" : "+D"(dest), "+c"(times) : "a"(value) : "memory");
            }
        }

        template<typename T>
        constexpr auto copy(T *dest, T const *src, size_t times) -> void
        {
            // We need to make sure that T is trivially copiable
            static_assert(std::is_trivially_copyable_v<T>);

            /* we can copy type which is bigger than register, because we can copy it in
             * different parts*/
            if constexpr (sizeof(T) % sizeof(u64) == 0) {
                times *= sizeof(T) / sizeof(u64);
                asm volatile("rep movsq" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            } else if constexpr (sizeof(T) % sizeof(u32) == 0) {
                times *= sizeof(T) / sizeof(u32);
                asm volatile("rep movsl" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            } else if constexpr (sizeof(T) % sizeof(u16) == 0) {
                times *= sizeof(T) / sizeof(u16);
                asm volatile("rep movsw" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            } else {
                times *= sizeof(T);
                asm volatile("rep movsb" : "+D"(dest), "+S"(src), "+c"(times) : : "memory");
            }
        }

        template<CanBeStoredInIntegral T>
        constexpr auto find(T const *location, T value, size_t limit) -> const T *
        {
            ++limit;

            if constexpr (sizeof(T) == sizeof(u8)) {
                asm volatile("repnz scasb; adc $-1, %0;"
                             : "+D"(location), "+c"(limit)
                             : "a"(value)
                             : "memory");
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                asm volatile("repnz scasw; adc $-2, %0;"
                             : "+D"(location), "+c"(limit)
                             : "a"(value)
                             : "memory");
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                asm volatile("repnz scasl; adc $-4, %0;"
                             : "+D"(location), "+c"(limit)
                             : "a"(value)
                             : "memory");
            } else if constexpr (sizeof(T) == sizeof(u64)) {
                asm volatile("repnz scasq; adc $-4, %0;"
                             : "+D"(location), "+c"(limit)
                             : "a"(value)
                             : "memory");
            }

            return location;
        }

        template<typename T>
        constexpr auto memcmp(T const *dest, T const *src, size_t length) -> bool
        {
            ++length;

            if constexpr (sizeof(T) % sizeof(u64) == 0) {
                length *= sizeof(T) / sizeof(u64);
                asm volatile("repe cmpsq; shr $3, %2;"
                             : "+D"(dest), "+S"(src), "+c"(length)
                             :
                             : "memory");
            } else if constexpr (sizeof(T) == sizeof(u32)) {
                length *= sizeof(T) / sizeof(u32);
                asm volatile("repe cmpsl; shr $2, %2;"
                             : "+D"(dest), "+S"(src), "+c"(length)
                             :
                             : "memory");
            } else if constexpr (sizeof(T) == sizeof(u16)) {
                length *= sizeof(T) / sizeof(u16);
                asm volatile("repe cmpsw; shr $1, %2;"
                             : "+D"(dest), "+S"(src), "+c"(length)
                             :
                             : "memory");
            } else {
                length *= sizeof(T);
                asm volatile("repe cmpsb;" : "+D"(dest), "+S"(src), "+c"(length) : : "memory");
            }

            return length == 0;
        }
    }// namespace amd64
#endif

    template<typename T>
    constexpr auto fill(T *dest, AutoCopyType<T> value, size_t times) -> void
    {
#if CERBLIB_AMD64
        if constexpr (std::is_trivially_copy_assignable_v<T> && CanBeStoredInIntegral<T>) {
            if (!std::is_constant_evaluated()) {
                return amd64::fill(dest, value, times);
            }
        }
#endif
        std::fill(dest, dest + times, value);
    }

    template<Iterable T>
    constexpr auto fill(T &dest, AutoCopyType<GetValueType<T>> value) -> void
    {
#if CERBLIB_AMD64
        if constexpr (
            RawAccessible<T> && ClassValueFastCopiable<T> &&
            CanBeStoredInIntegral<GetValueType<T>>) {
            if (!std::is_constant_evaluated()) {
                return amd64::fill(dest.getData(), value, dest.size());
            }
        }
#endif
        std::ranges::fill(dest, value);
    }

    template<typename T>
    constexpr auto copy(T *dest, T const *src, size_t times) -> void
    {
#if CERBLIB_AMD64
        if constexpr (std::is_trivially_copy_assignable_v<T>) {
            if (!std::is_constant_evaluated()) {
                return amd64::copy(dest, src, times);
            }
        }
#endif
        std::copy(src, src + times, dest);
    }

    template<Iterable T, Iterable U>
    constexpr auto copy(T &dest, U const &src) -> void
    {
        using value_type = GetValueType<T>;
        using value_type2 = GetValueType<U>;
        static_assert(std::is_same_v<value_type, value_type2>);

#if CERBLIB_AMD64
        if constexpr (
            RawAccessible<T> && RawAccessible<U> &&
            std::is_trivially_copy_assignable_v<value_type>) {
            auto const length = min<GetSizeType<T>>(std::size(dest), std::size(src));

            if (!std::is_constant_evaluated()) {
                return amd64::copy(dest.getData(), src.getData(), length);
            }
        }
#endif
        std::copy(src.begin(), src.end(), dest.begin());
    }

    template<typename T>
    CERBLIB_DECL auto find(T const *location, cerb::AutoCopyType<T> value, size_t limit)
        -> const T *
    {
#if CERBLIB_AMD64
        if constexpr (CanBeStoredInIntegral<T> && std::is_trivially_copy_assignable_v<T>) {
            return amd64::find(location, value, limit);
        }
#endif
        return std::find(location, location + limit, value);
    }

    template<Iterable T>
    CERBLIB_DECL auto find(T const &iterable_class, GetValueType<T> value2find) -> decltype(auto)
    {
#if CERBLIB_AMD64
        if constexpr (
            RawAccessible<T> && CanBeStoredInIntegral<GetValueType<T>> &&
            std::is_trivially_copy_assignable_v<T>) {
            if (std::is_constant_evaluated()) {
                return find(std::data(iterable_class), value2find, std::size(iterable_class));
            }
        }
#endif
        return std::ranges::find(iterable_class, value2find);
    }

    template<typename T>
    CERBLIB_DECL auto ptrdiff(const T first, const T last) -> size_t
    {
        return static_cast<size_t>(static_cast<ptrdiff_t>(last - first));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto strlen(const CharT *str) -> size_t
    {
        return ptrdiff(str, find(str, static_cast<CharT>(0), std::numeric_limits<u32>::max()));
    }
}// namespace cerb

#endif /* CERBERUS_MEMORY_HPP */
