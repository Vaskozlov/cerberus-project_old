#ifndef CERBERUS_CHAR_POINTER_HPP
#define CERBERUS_CHAR_POINTER_HPP

#include <cerberus/string_view.hpp>
#include <string>

namespace cerb::fmt
{
    template<CharacterLiteral CharT, CharacterLiteral T>
    CERBLIB_DECL auto convert(T const *ptr) -> std::basic_string<CharT>
    {
        std::basic_string<CharT> result{};

        CERBLIB_UNROLL_N(2)
        for (; *ptr != static_cast<T>(0); ++ptr) {
            result.push_back(static_cast<T>(*ptr));
        }

        return result;
    }
}// namespace cerb::fmt

#endif /* CERBERUS_CHAR_POINTER_HPP */
