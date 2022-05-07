#ifndef CERBERUS_MEMORY_HPP
#define CERBERUS_MEMORY_HPP

#include <cerberus/bit.hpp>
#include <cerberus/type.hpp>
#include <iterator>

namespace cerb {
#if CERBLIB_AMD64
    namespace amd64 {
        template<typename T>
        constexpr auto fill(T *dest, T value, size_t times) -> void {
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
        constexpr auto copy(T *dest, T const *src, size_t times) -> void {
            static_assert(std::is_trivially_copyable_v<T>);

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
        constexpr auto find(T const *location, T value, size_t limit) -> const T * {
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
        constexpr auto memcmp(T const *dest, T const *src, size_t length) -> bool {
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
    constexpr auto fill(T *dest, AutoCopyType<T> value, size_t times) -> void {
#if CERBLIB_AMD64
        if constexpr (std::is_trivially_copy_assignable_v<T> && CanBeStoredInIntegral<T>) {
            if CERBLIB_RUNTIME {
                return amd64::fill(dest, value, times);
            }
        }
#endif
        std::fill(dest, dest + times, value);
    }

    template<Iterable T>
    constexpr auto fill(T &dest, AutoCopyType<GetValueType<T>> value) -> void {
#if CERBLIB_AMD64
        if constexpr (
                RawAccessible<T> && ClassValueFastCopiable<T> &&
                CanBeStoredInIntegral<GetValueType<T>>) {
            if CERBLIB_RUNTIME {
                return amd64::fill(std::data(dest), value, std::size(dest));
            }
        }
#endif
        if constexpr (IsAnyOfV<GetValueType<T>, u8, i8>) {
            // stdlibc++ uses __builtin_memset for chars, so
            // std::ranges::fill does not work at compile time
            std::fill(dest.begin(), dest.end(), value);
        } else {
            std::ranges::fill(dest, value);
        }
    }

    template<typename T>
    constexpr auto copy(T *dest, T const *src, size_t times) -> void {
#if CERBLIB_AMD64
        if constexpr (std::is_trivially_copy_assignable_v<T>) {
            if CERBLIB_RUNTIME {
                return amd64::copy(dest, src, times);
            }
        }
#endif
        std::copy(src, src + times, dest);
    }

    template<Iterable T1, Iterable T2>
    constexpr auto copy(T1 &dest, T2 const &src) -> void {
        using value_type = GetValueType<T1>;
        using value_type2 = GetValueType<T2>;
        static_assert(std::is_same_v<value_type, value_type2>);

#if CERBLIB_AMD64
        constexpr bool suitable_for_fast_copy = RawAccessible<T1> && RawAccessible<T2> &&
                                                std::is_trivially_copy_assignable_v<value_type>;

        if constexpr (suitable_for_fast_copy) {
            auto const length = min<GetSizeType<T1>>(std::size(dest), std::size(src));

            if CERBLIB_RUNTIME {
                return amd64::copy(std::data(dest), std::size(src), length);
            }
        }
#endif
        std::copy(src.begin(), src.end(), dest.begin());
    }

    template<typename T>
    CERBLIB_DECL auto find(T const *location, AutoCopyType<T> value, size_t limit) -> T const * {
        [[maybe_unused]] constexpr bool suitable_for_fast_search =
                CanBeStoredInIntegral<T> && std::is_trivial_v<T>;

#if CERBLIB_AMD64
        if constexpr (suitable_for_fast_search) {
            if CERBLIB_RUNTIME {
                return amd64::find(location, value, limit);
            }
        }
#endif

        if CERBLIB_COMPILE_TIME {
            size_t counter = 0;

            while (counter != limit && *location != value) {
                ++location;
                ++counter;
            }

            return location;
        } else {
            return std::find(location, location + limit, value);
        }
    }

    template<Iterable T>
    CERBLIB_DECL auto find(T const &iterable_class, GetValueType<T> value_to_find) -> decltype(auto) {
#if CERBLIB_AMD64
        [[maybe_unused]] constexpr bool suitable_for_fast_search =
                RawAccessible<T> && CanBeStoredInIntegral<GetValueType<T>> &&
                std::is_trivial_v<GetValueType<T>>;

        if constexpr (suitable_for_fast_search) {
            if CERBLIB_RUNTIME {
                return find(std::data(iterable_class), value_to_find, std::size(iterable_class));
            }
        }
#endif
        return std::ranges::find(iterable_class, value_to_find);
    }

    template<Iterable T1, Iterable T2>
    CERBLIB_DECL auto equal(T1 const &lhs, T2 const &rhs) -> bool {
        using value_type = GetValueType<T1>;
        static_assert(std::is_same_v<value_type, GetValueType<T2>>);

        if (std::size(lhs) != std::size(rhs)) {
            return false;
        }

#if CERBLIB_AMD64
        constexpr bool can_be_fast_compared = RawAccessible<T1> && RawAccessible<T2> &&
                                              CanBeStoredInIntegral<value_type> &&
                                              std::is_trivial_v<value_type>;

        if constexpr (can_be_fast_compared) {
            if CERBLIB_RUNTIME {
                return amd64::memcmp(std::data(lhs), std::data(rhs), std::size(lhs));
            }
        }
#endif
        return std::ranges::equal(lhs, rhs);
    }

    template<typename T>
    CERBLIB_DECL auto equal(T const *lhs, T const *rhs, size_t length) -> bool {
#if CERBLIB_AMD64
        constexpr bool can_be_fast_compared = CanBeStoredInIntegral<T> && std::is_trivial_v<T>;

        if constexpr (can_be_fast_compared) {
            if CERBLIB_RUNTIME {
                return amd64::memcmp(lhs, rhs, length);
            }
        }
#endif
        return std::equal(lhs, lhs + length, rhs, rhs + length);
    }


    template<typename T>
    CERBLIB_DECL auto ptrdiff(T const first, T const last) -> size_t {
        return static_cast<size_t>(static_cast<ptrdiff_t>(last - first));
    }

    template<CharacterLiteral CharT, size_t Size>
    CERBLIB_DECL auto strlenForArray(const CharT (&)[Size]) -> size_t {
        return Size - 1;
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto strlenForPointer(CharT const *str) -> size_t
    {
        return ptrdiff(str, find(str, static_cast<CharT>(0), std::numeric_limits<u32>::max()));
    }

    template<typename T>
    CERBLIB_DECL auto strlen(T const &str) -> size_t {
        if constexpr (std::is_array_v<T>) {
            return strlenForArray<>(str);
        } else {
            return strlenForPointer(str);
        }
    }
}// namespace cerb

#endif /* CERBERUS_MEMORY_HPP */
