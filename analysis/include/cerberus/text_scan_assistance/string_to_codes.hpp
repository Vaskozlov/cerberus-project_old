#ifndef CERBERUS_STRING_TO_CODES_HPP
#define CERBERUS_STRING_TO_CODES_HPP

#include <cerberus/text_scan_assistance/scan_api.hpp>
#include <string>

namespace cerb::text
{
    CERBERUS_EXCEPTION(BasicStringToCodesTranslationError, BasicTextScanningException);

    template<CharacterLiteral CharT>
    CERBERUS_ANALYSIS_EXCEPTION(
        StringToCodesTranslationError, CharT, BasicStringToCodesTranslationError);

    template<CharacterLiteral CharT>
    class StringToCodes : private ScanApi<CharT>
    {
        CERBLIB_SCAN_API_ACCESS(CharT);

    public:
        constexpr auto parsedString() -> std::basic_string<CharT> &
        {
            return parsed_string;
        }

        constexpr StringToCodes(CharT string_opener, GeneratorForText<CharT> &text)
          : scan_api_t(text), string_begin_char(string_opener)
        {
            scan_api_t::beginScanning(string_begin_char);
        }

    private:
        constexpr auto onStart() -> text::ScanApiStatus override
        {
            checkStringStart();
            return text::ScanApiStatus::SKIP_CHAR;
        }

        constexpr auto processChar(CharT chr) -> void override
        {
            if (chr == lex::CharEnum<CharT>::Backlash) {
                parsed_string.push_back(
                    parseEscapeSequence({ { string_begin_char, string_begin_char } }));
            } else {
                parsed_string.push_back(chr);
            }
        }

        constexpr auto checkStringStart() const -> void
        {
            if (not isBeginOfString(getChar())) {
                throw StringToCodesTranslationError("Unable to open string!", getGenerator());
            }
        }

        CERBLIB_DECL auto isBeginOfString(CharT chr) const -> bool
        {
            return chr == string_begin_char;
        }

        static constexpr auto hexadecimal_chars = lex::HexadecimalCharsToInt<CharT>;

        std::basic_string<CharT> parsed_string{};
        CharT string_begin_char{};
    };

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto convertStringToCodes(CharT string_opener, GeneratorForText<CharT> &text)
        -> std::basic_string<CharT>
    {
        StringToCodes<CharT> string_to_codes{ string_opener, text };
        return std::move(string_to_codes.parsedString());
    }

#ifndef CERBERUS_HEADER_ONLY
    extern template class StringToCodes<char>;
    extern template class StringToCodes<char8_t>;
    extern template class StringToCodes<char16_t>;
    extern template class StringToCodes<char32_t>;
    extern template class StringToCodes<wchar_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::text

#endif /* CERBERUS_STRING_TO_CODES_HPP */
