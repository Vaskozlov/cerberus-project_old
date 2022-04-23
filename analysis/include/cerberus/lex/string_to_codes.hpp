#ifndef CERBERUS_STRING_TO_CODES_HPP
#define CERBERUS_STRING_TO_CODES_HPP

#include <cerberus/exception.hpp>
#include <cerberus/lex/char.hpp>
#include <cerberus/lex/scan_api.hpp>
#include <string>

namespace cerb::lex
{
    CERBERUS_EXCEPTION(StringToCodesTranslationError);

    template<CharacterLiteral CharT>
    class StringToCodes : private scan::ScanApi<false, CharT>
    {
        CERBLIB_SCAN_API_ACCESS(false, CharT);

    public:
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
                throw StringToCodesTranslationError("Unable to open string!");
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

    extern template class StringToCodes<char>;
    extern template class StringToCodes<char16_t>;
}// namespace cerb::lex

#endif /* CERBERUS_STRING_TO_CODES_HPP */
