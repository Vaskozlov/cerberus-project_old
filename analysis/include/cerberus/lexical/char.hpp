#ifndef CERBERUS_CHAR_HPP
#define CERBERUS_CHAR_HPP

#include "cerberus/cerberus.hpp"
#include "cerberus/type.hpp"

namespace cerb::lex {
    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isEndOfInput(CharT chr) -> bool
    {
        return chr == static_cast<CharT>(0);
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isUnderscore(CharT chr) -> bool
    {
        return chr == static_cast<CharT>('_');
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLayout(CharT chr) -> bool
    {
        return logicalAnd(chr > static_cast<CharT>(0), chr <= static_cast<CharT>(' '));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLayoutOrEndOfInput(CharT chr) -> bool
    {
        if constexpr (std::is_unsigned_v<CharT>) {
            return chr <= static_cast<CharT>(' ');
        } else {
            return logicalAnd(chr >= static_cast<CharT>(0), chr <= static_cast<CharT>(' '));
        }
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isUCLetter(CharT chr) -> bool
    {
        return logicalAnd(chr >= static_cast<CharT>('A'), chr <= static_cast<CharT>('Z'));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLCLetter(CharT chr) -> bool
    {
        return logicalAnd(chr >= static_cast<CharT>('a'), chr <= static_cast<CharT>('z'));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLetter(CharT chr) -> bool
    {
        return logicalOr(isUCLetter(chr), isLCLetter(chr));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isDigit(CharT chr) -> bool
    {
        return logicalAnd(chr >= static_cast<CharT>('0'), chr <= static_cast<CharT>('9'));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLetterOrDigit(CharT chr) -> bool
    {
        return logicalOr(isLetter(chr), isDigit(chr));
    }
}// namespace cerb::lex

#endif /* CERBERUS_CHAR_HPP */
