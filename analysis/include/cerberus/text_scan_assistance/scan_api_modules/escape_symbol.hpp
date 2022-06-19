#ifndef CERBERUS_ESCAPE_SYMBOL_HPP
#define CERBERUS_ESCAPE_SYMBOL_HPP

#include <cerberus/text_scan_assistance/scan_api_modules/notation_escape_symbol.hpp>
#include <cerberus/text_scan_assistance/scan_api_modules/skip_mode.hpp>

namespace cerb::text
{
    template<CharacterLiteral CharT>
    class EscapeSymbol
    {
    private:
        using scan_api_t = ScanApi<CharT>;
        using escape_symbol_pair = Pair<CharT, CharT, PairComparison::BY_FIRST_VALUE>;

        template<std::integral Int>
        CERBLIB_DECL static auto cast(Int value) -> CharT
        {
            return static_cast<CharT>(value);
        }

    public:
        constexpr auto parseEscapeSequence() -> CharT
        {
            CharT chr = scan_api.nextRawCharWithEoFCheck();
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

        CERBLIB_DEFAULT_NO_COPIABLE(EscapeSymbol);

        constexpr EscapeSymbol(
            scan_api_t &api_for_scan,
            std::initializer_list<escape_symbol_pair> const &other_symbols)
          : scan_api(api_for_scan), special_symbols(other_symbols)
        {}

    private:
        CERBLIB_DECL auto searchForSpecialUserSymbol(CharT chr) const -> CharT
        {
            auto location = cerb::find(special_symbols, chr);

            if (location == special_symbols.end()) {
                scan_api.throwException("Unable to match any escape sequence!");
            }

            return location->second;
        }

        scan_api_t &scan_api;
        std::initializer_list<escape_symbol_pair> const &special_symbols;
    };

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto processCharEscape(
        ScanApi<CharT> &scan_api,
        std::initializer_list<Pair<CharT, CharT, PairComparison::BY_FIRST_VALUE>> const
            &special_symbols) -> CharT
    {
        EscapeSymbol escape_symbol{ scan_api, special_symbols };
        return escape_symbol.parseEscapeSequence();
    }

#ifndef CERBERUS_HEADER_ONLY
    extern template class EscapeSymbol<char>;
    extern template class EscapeSymbol<char8_t>;
    extern template class EscapeSymbol<char16_t>;
    extern template class EscapeSymbol<char32_t>;
    extern template class EscapeSymbol<wchar_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::text

#endif /* CERBERUS_ESCAPE_SYMBOL_HPP */