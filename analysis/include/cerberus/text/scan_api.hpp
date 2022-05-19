#ifndef CERBERUS_SCAN_API_HPP
#define CERBERUS_SCAN_API_HPP

#include <cerberus/analysis/analysis_exception.hpp>
#include <cerberus/text/generator_for_text.hpp>

#define CERBLIB_SCAN_API_ACCESS(UseCleanChars, CharT)                                              \
    using scan_api_t = cerb::text::ScanApi<UseCleanChars, CharT>;                                  \
    using scan_api_t::getChar;                                                                     \
    using scan_api_t::getFutureChar;                                                               \
    using scan_api_t::nextChar;                                                                    \
    using scan_api_t::canContinueParsing;                                                          \
    using scan_api_t::getNextCharAndCheckForEoF;                                                   \
    using scan_api_t::getGenerator;                                                                \
    using scan_api_t::isGeneratorInitialized;                                                      \
    using scan_api_t::parseEscapeSequence;                                                         \
    using scan_api_t::setupGenerator

namespace cerb::text
{
    // NOLINTNEXTLINE
    CERBERUS_ENUM(ScanApiStatus, u8, SKIP_CHAR = 1, DO_NOT_SKIP_CHAR = 0);

    template<bool UseCleanChars, CharacterLiteral CharT>
    struct ScanApi
    {
        CERBERUS_ANALYSIS_EXCEPTION(ScanApiError, CharT, BasicTextAnalysisException);

        template<std::integral Int>
        CERBLIB_DECL static auto cast(Int value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        CERBLIB_DECL auto getGenerator() const -> GeneratorForText<CharT> const &
        {
            return text_generator;
        }

        CERBLIB_DECL auto getChar() const -> CharT
        {
            return text_generator.getCurrentChar();
        }

        CERBLIB_DECL auto getFutureChar() const -> CharT
        {
            return text_generator.getCurrentChar(1);
        }

        constexpr auto skip(size_t times) -> void
        {
            text_generator.template skip<UseCleanChars>(times);
        }

        constexpr auto nextChar() -> CharT
        {
            if constexpr (UseCleanChars) {
                return text_generator.getCleanChar();
            } else {
                return text_generator.getRawChar();
            }
        }

        CERBLIB_DECL auto getNextCharAndCheckForEoF() -> CharT
        {
            auto chr = nextChar();

            if (lex::isEoF(chr)) {
                throw ScanApiError("Unexpected EoF!", text_generator);
            }

            return chr;
        }

        constexpr auto canContinueParsing(CharT end_symbol) -> bool
        {
            using namespace lex;
            CharT chr = getChar();

            if (logicalAnd(lex::isEoF(chr), end_symbol != CharEnum<CharT>::EoF)) {
                throw ScanApiError(
                    "Unable to continue, because of unexpected EoF!", text_generator);
            }

            return chr != end_symbol;
        }

        CERBLIB_DECL auto isGeneratorInitialized() const -> bool
        {
            return text_generator.isInitialized();
        }

        constexpr auto setupGenerator() -> void
        {
            if (not isGeneratorInitialized()) {
                nextChar();
            }
        }

        constexpr auto parseEscapeSequence(
            std::initializer_list<Pair<CharT, CharT, PairComparison::BY_FIRST_VALUE>> const
                &special_symbols) -> CharT
        {
            CharT chr = getNextCharAndCheckForEoF();
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
                return convertStringToChar(octal_notation, 2);

            case cast('x'):
                return convertStringToChar(hexadecimal_notation, 2);

            case cast('u'):
                return convertStringToChar(hexadecimal_notation, 4);

            default:
                break;
            }

            return parseUserDefinedEscapeSymbols(chr, special_symbols);
        }

        constexpr auto beginScanning(CharT end_symbol) -> void
        {
            setupGenerator();

            if (start() == ScanApiStatus::SKIP_CHAR) {
                nextChar();
            }

            while (canContinueParsing(end_symbol)) {
                processChar(getChar());
                nextChar();
            }

            end();
        }

        CERBLIB_DECL virtual auto start() -> ScanApiStatus = 0;
        constexpr virtual auto processChar(CharT) -> void = 0;
        constexpr virtual auto end() -> void = 0;

        auto operator=(ScanApi const &) -> ScanApi & = default;
        auto operator=(ScanApi &&) noexcept -> ScanApi & = default;

        ScanApi() = default;
        ScanApi(ScanApi const &) = default;
        ScanApi(ScanApi &&) noexcept = default;

        constexpr explicit ScanApi(GeneratorForText<CharT> &generator_for_text)
          : text_generator(generator_for_text)
        {}

        virtual ~ScanApi() = default;

    private:
        CERBLIB_DECL auto parseUserDefinedEscapeSymbols(
            CharT chr,
            std::initializer_list<Pair<CharT, CharT, PairComparison::BY_FIRST_VALUE>> const
                &special_symbols) -> CharT
        {
            auto location = std::find(special_symbols.begin(), special_symbols.end(), chr);

            if (location == special_symbols.end()) {
                throw ScanApiError("Unable to match any escape sequence!", text_generator);
            }

            return location->second;
        }

        CERBLIB_DECL auto convertStringToChar(u32 notation, u32 length) -> CharT
        {
            using namespace lex;
            CharT resulted_char = CharEnum<CharT>::EoF;

            for (CERBLIB_UNUSED(u32) : Range(length)) {
                if (isOutOfNotation(getFutureChar(), notation)) {
                    break;
                }

                resulted_char *= cast(notation);
                resulted_char += convertSymbolToInt(getNextCharAndCheckForEoF());
            }

            return resulted_char;
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
            return cast(hexadecimal_chars[chr]);
        }

        static constexpr auto hexadecimal_chars = lex::HexadecimalCharsToInt<CharT>;
        GeneratorForText<CharT> &text_generator;
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct ScanApi<false, char>;
    extern template struct ScanApi<true, char>;

    extern template struct ScanApi<false, char8_t>;
    extern template struct ScanApi<true, char8_t>;

    extern template struct ScanApi<false, char16_t>;
    extern template struct ScanApi<true, char16_t>;

    extern template struct ScanApi<false, char32_t>;
    extern template struct ScanApi<true, char32_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::text

#endif /* CERBERUS_SCAN_API_HPP */
