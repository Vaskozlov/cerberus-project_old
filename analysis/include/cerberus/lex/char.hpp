#ifndef CERBERUS_CHAR_HPP
#define CERBERUS_CHAR_HPP

#include <cctype>
#include <cerberus/cerberus.hpp>
#include <cerberus/enum.hpp>
#include <cerberus/flat_map.hpp>
#include <cerberus/type.hpp>

#define CCHAR(x) static_cast<CharT>(x)

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    CERBERUS_ENUM(
        CharEnum, CharT, EoF = CCHAR('\0'), Tab = CCHAR('\t'),// NOLINT
        NewLine = CCHAR('\n'), CarriageReturn = CCHAR('\r'), Space = CCHAR(' '),
        Underscore = CCHAR('_'), DQM = CCHAR('\"'), Apostrophe = CCHAR('\''),
        Backslash = CCHAR('\\'));

    template<CharacterLiteral CharT>
    constexpr FlatMap<CharT, u16, 22> HexadecimalCharsToInt{
        { CCHAR('0'), 0 },  { CCHAR('1'), 1 },  { CCHAR('2'), 2 },  { CCHAR('3'), 3 },
        { CCHAR('4'), 4 },  { CCHAR('5'), 5 },  { CCHAR('6'), 6 },  { CCHAR('7'), 7 },
        { CCHAR('8'), 8 },  { CCHAR('9'), 9 },  { CCHAR('a'), 10 }, { CCHAR('b'), 11 },
        { CCHAR('c'), 12 }, { CCHAR('d'), 13 }, { CCHAR('e'), 14 }, { CCHAR('f'), 15 },
        { CCHAR('A'), 10 }, { CCHAR('B'), 11 }, { CCHAR('C'), 12 }, { CCHAR('D'), 13 },
        { CCHAR('E'), 14 }, { CCHAR('F'), 15 }
    };

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isEoF(CharT chr) -> bool
    {
        return chr == CharEnum<CharT>::EoF;
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLayout(CharT chr) -> bool
    {
        return logicalAnd(chr > CharEnum<CharT>::EoF, chr <= CharEnum<CharT>::Space);
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isDigit(CharT chr) -> bool
    {
        return logicalAnd(chr >= CCHAR('0'), chr <= CCHAR('9'));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isUcLetter(CharT chr) -> bool
    {
        return logicalAnd(chr >= CCHAR('A'), chr <= CCHAR('Z'));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLcLetter(CharT chr) -> bool
    {
        return logicalAnd(chr >= CCHAR('a'), chr <= CCHAR('z'));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLetter(CharT chr) -> bool
    {
        return logicalOr(isUcLetter(chr), isLcLetter(chr));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLetterOrDigit(CharT chr) -> bool
    {
        return logicalOr(isDigit(chr), isLetter(chr));
    }
}// namespace cerb::lex

#undef CCHAR

#endif /* CERBERUS_CHAR_HPP */
