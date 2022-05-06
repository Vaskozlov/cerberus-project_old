#ifndef CERBERUS_STRING_TO_CODES_HPP
#define CERBERUS_STRING_TO_CODES_HPP

#include <cerberus/exception.hpp>
#include <cerberus/lex/char.hpp>
#include <cerberus/lex/scan_api.hpp>
#include <string>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    class StringToCodes : private scan::ScanApi<false, CharT>
    {
        CERBLIB_SCAN_API_ACCESS(false, CharT);

    public:
        CERBERUS_ANALYSIS_EXCEPTION(
            StringToCodesTranslationError, CharT, BasicLexicalAnalysisException);

        constexpr auto parseString() -> std::basic_string<CharT> &
        {
            setupGenerator();
            checkStringStart();

            while (canContinueParsing(string_begin_char)) {
                CharT chr = getChar();
                processChar(chr);
            }

            return parsed_string;
        }

        StringToCodes() = default;

        constexpr StringToCodes(CharT string_opener, GeneratorForText<CharT> &text)
          : scan_api_t(text), string_begin_char(string_opener)
        {}

    private:
        constexpr auto processChar(CharT chr) -> void
        {
            if (chr == CharEnum<CharT>::Backlash) {
                parsed_string.push_back(parseEscapeSequence(string_begin_char));
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

        static constexpr auto hexadecimal_chars = HexadecimalCharsToInt<CharT>;

        std::basic_string<CharT> parsed_string{};
        CharT string_begin_char{};
    };

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto convertStringToCodes(CharT string_opener, GeneratorForText<CharT> &text)
        -> std::basic_string<CharT>
    {
        StringToCodes<CharT> string_to_codes{ string_opener, text };
        return std::move(string_to_codes.parseString());
    }

#ifndef CERBERUS_HEADER_ONLY
    extern template class StringToCodes<char>;
    extern template class StringToCodes<char8_t>;
    extern template class StringToCodes<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::lex

#endif /* CERBERUS_STRING_TO_CODES_HPP */
