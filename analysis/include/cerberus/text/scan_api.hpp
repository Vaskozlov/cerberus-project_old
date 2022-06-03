#ifndef CERBERUS_SCAN_API_HPP
#define CERBERUS_SCAN_API_HPP

#include <cerberus/analysis/analysis_exception.hpp>
#include <cerberus/text/generator_for_text.hpp>
#include <cerberus/text/scan_api_modules/escape_symbol.hpp>

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

    CERBERUS_EXCEPTION(BasicScanApiError, BasicTextAnalysisException);

    template<CharacterLiteral CharT>
    CERBERUS_ANALYSIS_EXCEPTION(ScanApiError, CharT, BasicScanApiError);

    template<bool UseCleanChars, CharacterLiteral CharT>
    struct ScanApi
    {
        friend EscapeSymbol<UseCleanChars, CharT>;
        friend NotationEscapeSymbol<UseCleanChars, CharT>;

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
                throwException("Unexpected EoF!");
            }

            return chr;
        }

        constexpr auto canContinueParsing(CharT end_symbol) -> bool
        {
            using namespace lex;
            CharT chr = getChar();

            if (logicalAnd(lex::isEoF(chr), end_symbol != CharEnum<CharT>::EoF)) {
                throwException("Unable to continue, because of unexpected EoF!");
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
            return processCharEscape(*this, special_symbols);
        }

        constexpr auto beginScanning(CharT end_symbol) -> void
        {
            setupGenerator();

            if (onStart() == ScanApiStatus::SKIP_CHAR) {
                nextChar();
            }

            while (canContinueParsing(end_symbol)) {
                processChar(getChar());
                nextChar();
            }

            onEnd();
        }

        CERBLIB_DECL virtual auto onStart() -> ScanApiStatus = 0;
        constexpr virtual auto onEnd() -> void
        {
            // empty method
        }

        constexpr virtual auto processChar(CharT) -> void = 0;

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
        constexpr auto throwException(string_view const &message) -> void
        {
            throw ScanApiError(message, text_generator);
        }

        GeneratorForText<CharT> &text_generator;
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct ScanApiError<char>;
    extern template struct ScanApiError<char8_t>;
    extern template struct ScanApiError<char16_t>;
    extern template struct ScanApiError<char32_t>;
    extern template struct ScanApiError<wchar_t>;

    extern template struct ScanApi<false, char>;
    extern template struct ScanApi<true, char>;

    extern template struct ScanApi<false, char8_t>;
    extern template struct ScanApi<true, char8_t>;

    extern template struct ScanApi<false, char16_t>;
    extern template struct ScanApi<true, char16_t>;

    extern template struct ScanApi<false, char32_t>;
    extern template struct ScanApi<true, char32_t>;

    extern template struct ScanApi<false, wchar_t>;
    extern template struct ScanApi<true, wchar_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::text

#endif /* CERBERUS_SCAN_API_HPP */
