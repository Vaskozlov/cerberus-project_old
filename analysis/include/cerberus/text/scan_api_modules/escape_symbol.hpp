#ifndef CERBERUS_ESCAPE_SYMBOL_HPP
#define CERBERUS_ESCAPE_SYMBOL_HPP

#include <cerberus/text/scan_api_modules/notation_escape_symbol.hpp>
#include <cerberus/text/scan_api_modules/skip_mode.hpp>

namespace cerb::text
{
    template<SkipMode Mode, CharacterLiteral CharT>
    struct EscapeSymbol
    {
        using symbol_pair = Pair<CharT, CharT, PairComparison::BY_FIRST_VALUE>;
        using scan_api_t = ScanApi<Mode, CharT>;

        template<std::integral Int>
        CERBLIB_DECL static auto cast(Int value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        constexpr auto parseEscapeSequence() -> CharT
        {
            CharT chr = scan_api.getNextCharAndCheckForEoF();
            constexpr size_t octal_notation = 8;
            constexpr size_t hexadecimal_notation = 16;

            switch (chr) {
            case cast('\\'):
                return cast('\\');

            case cast('\''):
                return cast('\'');

            case cast('\"'):
                return cast('\"');

            case cast('t'):
                return cast('\t');

            case cast('n'):
                return cast('\n');

            case cast('r'):
                return cast('\r');

            case cast('f'):
                return cast('\f');

            case cast('b'):
                return cast('\b');

            case cast('0'):
                return convertCharEscape<octal_notation>(scan_api, 2);

            case cast('x'):
                return convertCharEscape<hexadecimal_notation>(scan_api, 2);

            case cast('u'):
                return convertCharEscape<hexadecimal_notation>(scan_api, 4);

            default:
                break;
            }

            return searchForSpecialUserSymbol(chr);
        }

        EscapeSymbol() = default;
        constexpr EscapeSymbol(
            scan_api_t &api_for_scan,
            std::initializer_list<symbol_pair> const &other_symbols)
          : scan_api(api_for_scan), special_symbols(other_symbols)
        {}

    private:
        CERBLIB_DECL auto searchForSpecialUserSymbol(CharT chr) const -> CharT
        {
            auto location = std::find(special_symbols.begin(), special_symbols.end(), chr);

            if (location == special_symbols.end()) {
                scan_api.throwException("Unable to match any escape sequence!");
            }

            return location->second;
        }

        scan_api_t &scan_api;
        std::initializer_list<symbol_pair> const &special_symbols;
    };

    template<SkipMode Mode, CharacterLiteral CharT>
    CERBLIB_DECL auto processCharEscape(
        ScanApi<Mode, CharT> &scan_api,
        std::initializer_list<Pair<CharT, CharT, PairComparison::BY_FIRST_VALUE>> const
            &special_symbols) -> CharT
    {
        EscapeSymbol escape_symbol{ scan_api, special_symbols };
        return escape_symbol.parseEscapeSequence();
    }

#ifndef CERBERUS_HEADER_ONLY
    extern template struct EscapeSymbol<RAW_CHARS, char>;
    extern template struct EscapeSymbol<CLEAN_CHARS, char>;

    extern template struct EscapeSymbol<RAW_CHARS, char8_t>;
    extern template struct EscapeSymbol<CLEAN_CHARS, char8_t>;

    extern template struct EscapeSymbol<RAW_CHARS, char16_t>;
    extern template struct EscapeSymbol<CLEAN_CHARS, char16_t>;

    extern template struct EscapeSymbol<RAW_CHARS, char32_t>;
    extern template struct EscapeSymbol<CLEAN_CHARS, char32_t>;

    extern template struct EscapeSymbol<RAW_CHARS, wchar_t>;
    extern template struct EscapeSymbol<CLEAN_CHARS, wchar_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::text

#endif /* CERBERUS_ESCAPE_SYMBOL_HPP */
