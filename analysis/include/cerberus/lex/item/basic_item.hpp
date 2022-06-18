#ifndef CERBERUS_BASIC_ITEM_HPP
#define CERBERUS_BASIC_ITEM_HPP

#include <cerberus/lazy_executor.hpp>
#include <cerberus/lex/lexical_analysis_exception.hpp>
#include <cerberus/lex/token.hpp>
#include <cerberus/small_vector.hpp>
#include <cerberus/string_container.hpp>
#include <cerberus/text_scan_assistance/generator_for_text.hpp>
#include <cerberus/text_scan_assistance/string_to_codes.hpp>

#define CERBLIB_BASIC_ITEM_ACCESS(CharT)                                                           \
    using basic_item_t = BasicItem<CharT>;                                                         \
    using basic_item_t::analysis_globals;                                                          \
    using basic_item_t::cast;                                                                      \
    using basic_item_t::flags;                                                                     \
    using typename basic_item_t::ScanResult

#define CERBLIB_BASIC_ITEM_ARGS cerb::lex::AnalysisGlobals<CharT> &analysis_parameters
#define CERBLIB_CONSTRUCT_BASIC_ITEM basic_item_t(analysis_parameters)

namespace cerb::lex
{
    CERBERUS_ENUM(// NOLINTNEXTLINE
        ItemFlags, u32, NONE = 0b0, STAR = 0b1, PLUS = 0b10, QUESTION = 0b1'00, FIXED = 0b1'000,
        PREFIX = 0b10'000, REVERSE = 0b100'000, NONTERMINAL = 0b10'000'000);

    // NOLINTNEXTLINE
    CERBERUS_ENUM(ScanResultStatus, u16, NONE = 0b0, SUCCESS = 0b1, FAILURE = 0b10);

    template<CharacterLiteral CharT>
    struct AnalysisGlobals
    {
        using string_container_t = StringContainer<CharT, size_t, true>;
        using tokens_storage_iterator = typename string_container_t::tokens_storage_iterator;

        constexpr auto containsNonterminal(BasicStringView<CharT> const &nonterminal) const -> bool
        {
            auto longest = nonterminals.findLongestString(nonterminal);
            return nonterminals.contains(longest);
        }

        constexpr auto emplaceNonterminal(std::basic_string<CharT> &&str, size_t id)
            -> std::pair<tokens_storage_iterator, bool>
        {
            return nonterminals.emplace(std::move(str), id);
        }

        constexpr auto emplaceNonterminal(std::basic_string<CharT> const &str, size_t id)
            -> std::pair<tokens_storage_iterator, bool>
        {
            return nonterminals.emplace(str, id);
        }

        AnalysisGlobals() = default;

        string_container_t nonterminals{};
        LazyExecutor<> lazy_executor{ 2 };
    };

    template<CharacterLiteral CharT>
    struct BasicItem
    {
        struct ScanResult
        {
            ScanResultStatus status{};
        };

        template<std::integral Int>
        CERBLIB_DECL static auto cast(Int value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        CERBLIB_DECL virtual auto scan(text::GeneratorForText<CharT>) const -> ScanResult = 0;
        virtual constexpr auto postInitializationSetup() -> void = 0;

        BasicItem() = default;
        BasicItem(BasicItem const &) = default;
        BasicItem(BasicItem &&) noexcept = default;

        constexpr explicit BasicItem(AnalysisGlobals<CharT> &analysis_parameters)
          : analysis_globals(analysis_parameters)
        {}

        virtual ~BasicItem() = default;

        auto operator=(BasicItem const &) -> BasicItem & = default;
        auto operator=(BasicItem &&) noexcept -> BasicItem & = default;

        AnalysisGlobals<CharT> &analysis_globals;
        ItemFlags flags{ ItemFlags::NONE };
    };

    // NOLINTNEXTLINE
    inline LazyExecutor LexicalAnalysisLazyExecutor{ max<uint>(
        2U, std::thread::hardware_concurrency()) };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct AnalysisGlobals<char>;
    extern template struct AnalysisGlobals<char8_t>;
    extern template struct AnalysisGlobals<char16_t>;

    extern template struct BasicItem<char>;
    extern template struct BasicItem<char8_t>;
    extern template struct BasicItem<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::lex

#endif /* CERBERUS_BASIC_ITEM_HPP */
