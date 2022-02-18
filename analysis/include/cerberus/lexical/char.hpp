#ifndef CERBERUS_CHAR_HPP
#define CERBERUS_CHAR_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/enum.hpp>
#include <cerberus/flat_map.hpp>
#include <cerberus/type.hpp>

namespace cerb::lex
{
#define CCAST(x) static_cast<CharT>(x)

    template<CharacterLiteral CharT>
    CERBERUS_ENUM(
        CharsEnum, CharT, EoF = CCAST('\0'), Tab = CCAST('\t'),// NOLINT
        NewLine = CCAST('\n'), CarriageReturn = CCAST('\r'), Space = CCAST(' '),
        Underscore = CCAST('_'), DQM = CCAST('\"'), Apostrophe = CCAST('\''),
        Backslash = CCAST('\\'));

    template<CharacterLiteral CharT>
    constexpr FlatMap<CharT, u16, 22> HexadecimalCharsToInt{
        { CCAST('0'), 0 },  { CCAST('1'), 1 },  { CCAST('2'), 2 },  { CCAST('3'), 3 },
        { CCAST('4'), 4 },  { CCAST('5'), 5 },  { CCAST('6'), 6 },  { CCAST('7'), 7 },
        { CCAST('8'), 8 },  { CCAST('9'), 9 },  { CCAST('a'), 10 }, { CCAST('b'), 11 },
        { CCAST('c'), 12 }, { CCAST('d'), 13 }, { CCAST('e'), 14 }, { CCAST('f'), 15 },
        { CCAST('A'), 10 }, { CCAST('B'), 11 }, { CCAST('C'), 12 }, { CCAST('D'), 13 },
        { CCAST('E'), 14 }, { CCAST('F'), 15 }
    };

    using char_enum_t = CharsEnum<char>;
    using char8_enum_t = CharsEnum<char8_t>;
    using char16_enum_t = CharsEnum<char16_t>;
    using char32_enum_t = CharsEnum<char32_t>;
    using wchar_enum_t = CharsEnum<wchar_t>;

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isEoF(CharT chr) -> bool
    {
        return chr == CharsEnum<CharT>::EoF;
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isUnderscore(CharT chr) -> bool
    {
        return chr == CharsEnum<CharT>::Underscore;
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLayout(CharT chr) -> bool
    {
        return logicalAnd(chr > CharsEnum<CharT>::EoF, chr <= CharsEnum<CharT>::Space);
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLayoutOrEndOfInput(CharT chr) -> bool
    {
        if constexpr (std::is_unsigned_v<CharT>) {
            return chr <= CharsEnum<CharT>::Space;
        } else {
            return logicalAnd(chr >= CharsEnum<CharT>::EoF, chr <= CharsEnum<CharT>::Space);
        }
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isUcLetter(CharT chr) -> bool
    {
        return logicalAnd(chr >= CCAST('A'), chr <= CCAST('Z'));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLcLetter(CharT chr) -> bool
    {
        return logicalAnd(chr >= CCAST('a'), chr <= CCAST('z'));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLetter(CharT chr) -> bool
    {
        return logicalOr(isUcLetter(chr), isLcLetter(chr));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isDigit(CharT chr) -> bool
    {
        return logicalAnd(chr >= CCAST('0'), chr <= CCAST('9'));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLetterOrDigit(CharT chr) -> bool
    {
        return logicalOr(isLetter(chr), isDigit(chr));
    }

#undef CCAST

}// namespace cerb::lex

#endif /* CERBERUS_CHAR_HPP */
