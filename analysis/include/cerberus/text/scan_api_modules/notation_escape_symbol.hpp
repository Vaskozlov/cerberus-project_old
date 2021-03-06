#ifndef CERBERUS_NOTATION_ESCAPE_SYMBOL_HPP
#define CERBERUS_NOTATION_ESCAPE_SYMBOL_HPP

#include <cerberus/text/generator_for_text.hpp>
#include <cerberus/text/scan_api_modules/skip_mode.hpp>

namespace cerb::text
{
    template<SkipMode Mode, CharacterLiteral CharT>
    struct ScanApi;

    template<SkipMode Mode, CharacterLiteral CharT>
    struct NotationEscapeSymbol
    {
        using scan_api_t = ScanApi<Mode, CharT>;

        CERBLIB_DECL auto convert() -> CharT
        {
            using namespace lex;
            CharT final_char = CharEnum<CharT>::EoF;

            for (u32 i = 0; i != length; ++i) {
                if (isOutOfNotation(scan_api.getFutureRawChar(), notation)) {
                    break;
                }

                final_char = applySymbolToResult(final_char);
            }

            return final_char;
        }

        NotationEscapeSymbol() = default;
        constexpr NotationEscapeSymbol(
            scan_api_t &api_for_scan, u32 escape_notation, u32 escape_length)
          : scan_api(api_for_scan), notation(escape_notation), length(escape_length)
        {}

    private:
        CERBLIB_DECL auto applySymbolToResult(CharT final_char) const -> CharT
        {
            CharT chr = scan_api.getNextCharAndCheckForEoF();

            final_char *= static_cast<CharT>(notation);
            final_char += convertSymbolToInt(chr);

            return final_char;
        }

        CERBLIB_DECL static auto isOutOfNotation(CharT chr, u32 notation) -> bool
        {
            if (not hexadecimal_chars.contains(chr)) {
                return true;
            }

            auto code = convertSymbolToInt(chr);
            return static_cast<i32>(code) >= static_cast<i32>(notation);
        }

        CERBLIB_DECL static auto convertSymbolToInt(CharT chr) -> CharT
        {
            return static_cast<CharT>(hexadecimal_chars[chr]);
        }

        static constexpr auto hexadecimal_chars = lex::HexadecimalCharsToInt<CharT>;

        scan_api_t &scan_api;
        u32 notation{};
        u32 length{};
    };

    template<size_t Notation, SkipMode Mode, CharacterLiteral CharT>
    CERBLIB_DECL auto convertCharEscape(ScanApi<Mode, CharT> &scan_api, u32 length) -> CharT
    {
        constexpr size_t hexadecimal_notation = 16;
        static_assert(Notation <= hexadecimal_notation);

        NotationEscapeSymbol char_escape{ scan_api, Notation, length };
        return char_escape.convert();
    }

#ifndef CERBERUS_HEADER_ONLY
    extern template struct NotationEscapeSymbol<RAW_CHARS, char>;
    extern template struct NotationEscapeSymbol<CLEAN_CHARS, char>;

    extern template struct NotationEscapeSymbol<RAW_CHARS, char8_t>;
    extern template struct NotationEscapeSymbol<CLEAN_CHARS, char8_t>;

    extern template struct NotationEscapeSymbol<RAW_CHARS, char16_t>;
    extern template struct NotationEscapeSymbol<CLEAN_CHARS, char16_t>;

    extern template struct NotationEscapeSymbol<RAW_CHARS, char32_t>;
    extern template struct NotationEscapeSymbol<CLEAN_CHARS, char32_t>;

    extern template struct NotationEscapeSymbol<RAW_CHARS, wchar_t>;
    extern template struct NotationEscapeSymbol<CLEAN_CHARS, wchar_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::text

#endif /* CERBERUS_NOTATION_ESCAPE_SYMBOL_HPP */
