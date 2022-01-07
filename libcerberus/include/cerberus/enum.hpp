#ifndef CERBERUS_ENUM_HPP
#define CERBERUS_ENUM_HPP

#include <cerberus/bits.hpp>
#include <cerberus/cerberus.hpp>

namespace cerb {
    template<Enum T>
    constexpr auto addFlag(T &flags, T new_flag) -> T
    {
        flags = static_cast<T>(convert2UnsignedInt(flags) | convert2UnsignedInt(new_flag));
        return flags;
    }

    template<Enum T>
    CERBLIB_DECL auto isFlagSet(T flags, T new_flag) -> bool
    {
        return (convert2UnsignedInt(flags) & convert2UnsignedInt(new_flag)) ==
               convert2UnsignedInt(new_flag);
    }

    template<Enum T>
    constexpr auto removeFlag(T &flags, T flag2remove) -> T
    {
        flags = static_cast<T>(convert2UnsignedInt(flags) & (~convert2UnsignedInt(flag2remove)));
        return flags;
    }
}// namespace cerb

#endif /* CERBERUS_ENUM_HPP */
