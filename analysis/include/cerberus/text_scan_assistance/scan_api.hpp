#ifndef CERBERUS_SCAN_API_HPP
#define CERBERUS_SCAN_API_HPP

#include <cerberus/analysis/analysis_exception.hpp>
#include <cerberus/text_scan_assistance/generator_for_text.hpp>
#include <cerberus/text_scan_assistance/scan_api_modules/comment_skipper.hpp>
#include <cerberus/text_scan_assistance/scan_api_modules/escape_symbol.hpp>

#define CERBLIB_SCAN_API_ACCESS(CharT)                                                             \
    using scan_api_t = cerb::text::ScanApi<CharT>;                                                 \
    using scan_api_t::getChar;                                                                     \
    using scan_api_t::getFutureRawChar;                                                            \
    using scan_api_t::nextChar;                                                                    \
    using scan_api_t::nextRawChar;                                                                 \
    using scan_api_t::nextCleanChar;                                                               \
    using scan_api_t::canContinueParsing;                                                          \
    using scan_api_t::nextCharWithEoFCheck;                                                        \
    using scan_api_t::nextRawCharWithEoFCheck;                                                     \
    using scan_api_t::nextCleanCharWithEoFCheck;                                                   \
    using scan_api_t::getGenerator;                                                                \
    using scan_api_t::isGeneratorInitialized;                                                      \
    using scan_api_t::parseEscapeSequence;                                                         \
    using scan_api_t::setupGenerator

namespace cerb::text
{
    // NOLINTNEXTLINE
    CERBERUS_ENUM(ScanApiStatus, u8, SKIP_CHAR = 1, DO_NOT_SKIP_CHAR = 0);

    CERBERUS_EXCEPTION(BasicScanApiError, BasicTextScanningException);

    template<CharacterLiteral CharT>
    CERBERUS_ANALYSIS_EXCEPTION(ScanApiError, CharT, BasicScanApiError);

    template<CharacterLiteral CharT>
    struct ScanApi
    {
        friend CommentSkipper<CharT>;
        friend EscapeSymbol<CharT>;
        friend NotationEscapeSymbol<CharT>;

        using symbol_pair = Pair<CharT, CharT, PairComparison::BY_FIRST_VALUE>;

        template<std::integral Int>
        CERBLIB_DECL static auto cast(Int value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        CERBLIB_DECL auto getGenerator() const -> GeneratorForText<CharT> const &
        {
            return text_generator;
        }

        CERBLIB_DECL auto getLine() const -> size_t
        {
            auto const &generator = getGenerator();
            return generator.getLine();
        }

        CERBLIB_DECL auto getCharPosition() const -> size_t
        {
            auto const &generator = getGenerator();
            return generator.getColumn();
        }

        CERBLIB_DECL auto getFilename() const -> BasicStringView<char> const &
        {
            auto const &generator = getGenerator();
            return generator.getFilename();
        }

        CERBLIB_DECL auto getChar() const -> CharT
        {
            return text_generator.getCurrentChar();
        }

        CERBLIB_DECL auto getFutureRawChar() const -> CharT
        {
            return text_generator.getCurrentChar(1);
        }

        CERBLIB_DECL auto changeMode(SkipMode new_mode)
        {
            skip_mode = new_mode;
        }

        constexpr auto skip(size_t times) -> void
        {
            text_generator.skip(times, skip_mode);
        }

        constexpr auto nextChar() -> CharT
        {
            if (skip_mode == CLEAN_CHARS) {
                return nextCleanChar();
            }

            return nextRawChar();
        }

        constexpr auto nextRawChar() -> CharT
        {
            return text_generator.getRawChar();
        }

        constexpr auto nextCleanChar() -> CharT
        {
            return text_generator.getCleanChar();
        }

        CERBLIB_DECL auto nextCharWithEoFCheck() -> CharT
        {
            if (skip_mode == CLEAN_CHARS) {
                return nextCleanCharWithEoFCheck();
            }

            return nextRawCharWithEoFCheck();
        }

        CERBLIB_DECL auto nextRawCharWithEoFCheck() -> CharT
        {
            CharT chr = nextRawChar();
            return returnCharIfNotEoF(chr);
        }

        CERBLIB_DECL auto nextCleanCharWithEoFCheck() -> CharT
        {
            CharT chr = nextCleanChar();
            return returnCharIfNotEoF(chr);
        }

        constexpr auto canContinueParsing(CharT end_symbol) -> bool
        {
            using namespace lex;
            CharT chr = getChar();

            if (logicalAnd(isEoF(chr), end_symbol != CharEnum<CharT>::EoF)) {
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

        CERBLIB_DECL auto
            parseEscapeSequence(std::initializer_list<symbol_pair> const &special_symbols) -> CharT
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
                comment_skipper.skipComment();
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

        auto operator=(ScanApi const &) -> ScanApi & = delete;
        auto operator=(ScanApi &&) noexcept -> ScanApi & = default;

        ScanApi(ScanApi const &) = delete;
        ScanApi(ScanApi &&) noexcept = default;

        constexpr explicit ScanApi(
            GeneratorForText<CharT> &generator_for_text,
            SkipMode mode = RAW_CHARS,
            BasicStringView<CharT> const &single_line_comment = {},
            BasicStringView<CharT> const &multiline_comment_begin = {},
            BasicStringView<CharT> const &multiline_comment_end = {})
          : text_generator(generator_for_text), comment_skipper(
                                                    generator_for_text,
                                                    single_line_comment,
                                                    multiline_comment_begin,
                                                    multiline_comment_end),
            skip_mode(mode)
        {}

        virtual ~ScanApi() = default;

    private:
        CERBLIB_DECL auto returnCharIfNotEoF(CharT chr) const -> CharT
        {
            if (lex::isEoF(chr)) {
                throwException("Unexpected EoF!");
            }

            return chr;
        }

        constexpr auto throwException(string_view const &message) const -> void
        {
            throw ScanApiError(message, text_generator);
        }

        GeneratorForText<CharT> &text_generator;
        CommentSkipper<CharT> comment_skipper;
        SkipMode skip_mode{ RAW_CHARS };
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct ScanApiError<char>;
    extern template struct ScanApiError<char8_t>;
    extern template struct ScanApiError<char16_t>;
    extern template struct ScanApiError<char32_t>;
    extern template struct ScanApiError<wchar_t>;

    extern template struct ScanApi<char>;
    extern template struct ScanApi<char8_t>;
    extern template struct ScanApi<char16_t>;
    extern template struct ScanApi<char32_t>;
    extern template struct ScanApi<wchar_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::text

#endif /* CERBERUS_SCAN_API_HPP */
