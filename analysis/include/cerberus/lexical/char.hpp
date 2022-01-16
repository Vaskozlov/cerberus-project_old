#ifndef CERBERUS_CHAR_HPP
#define CERBERUS_CHAR_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/flat_map.hpp>
#include <cerberus/type.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    constexpr FlatMap<CharT, u16, 22> HexadecimalCharsToInt{
        { static_cast<CharT>('0'), 0 },  { static_cast<CharT>('1'), 1 },
        { static_cast<CharT>('2'), 2 },  { static_cast<CharT>('3'), 3 },
        { static_cast<CharT>('4'), 4 },  { static_cast<CharT>('5'), 5 },
        { static_cast<CharT>('6'), 6 },  { static_cast<CharT>('7'), 7 },
        { static_cast<CharT>('8'), 8 },  { static_cast<CharT>('9'), 9 },
        { static_cast<CharT>('a'), 10 }, { static_cast<CharT>('b'), 11 },
        { static_cast<CharT>('c'), 12 }, { static_cast<CharT>('d'), 13 },
        { static_cast<CharT>('e'), 14 }, { static_cast<CharT>('f'), 15 },
        { static_cast<CharT>('A'), 10 }, { static_cast<CharT>('B'), 11 },
        { static_cast<CharT>('C'), 12 }, { static_cast<CharT>('D'), 13 },
        { static_cast<CharT>('E'), 14 }, { static_cast<CharT>('F'), 15 }
    };

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
