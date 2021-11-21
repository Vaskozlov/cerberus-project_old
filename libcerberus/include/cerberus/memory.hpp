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
    }// namespace private_
#endif

    template<typename T>
    constexpr auto memset(T *dest, const T &value, size_t times) -> void
    {
#ifdef __x86_64__
        if (!std::is_constant_evaluated() && std::is_trivially_copyable_v<T>) {
            private_::memset(dest, value, times);
        }
#endif
        for (const T *end_of_dest = dest + times; dest != end_of_dest; ++dest) {
            *dest = value;
        }
    }

    template<typename T>
    constexpr auto memset(T &dest, const auto &value) -> void
    {
#ifdef __x86_64__
        if constexpr (cerb::DataAccessible<T>) {
            if (!std::is_constant_evaluated() && std::is_trivially_copyable_v<T>) {
                private_::memset(dest.data(), value, dest.size());
            }
        }
#endif
        static_assert(cerb::Iterable<T>);

        for (auto &elem : dest) {
            elem = value;
        }
    }

}// namespace cerb

#endif /* CERBERUS_MEMORY_HPP */
