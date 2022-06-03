#ifndef CERBERUS_BASIC_ITEM_HPP
#define CERBERUS_BASIC_ITEM_HPP

#include <cerberus/lazy_executor.hpp>
#include <cerberus/lex/lexical_analysis_exception.hpp>
#include <cerberus/string_pool.hpp>
#include <cerberus/text/generator_for_text.hpp>
#include <cerberus/text/string_to_codes.hpp>

#define CERBLIB_BASIC_ITEM_ACCESS(CharT)                                                           \
    using basic_item_t = BasicItem<CharT>;                                                         \
    using basic_item_t::analysis_globals;                                                          \
    using basic_item_t::cast;                                                                      \
    using basic_item_t::flags

#define CERBLIB_BASIC_ITEM_ARGS cerb::lex::AnalysisGlobals<CharT> &analysis_parameters
#define CERBLIB_CONSTRUCT_BASIC_ITEM basic_item_t(analysis_parameters)

namespace cerb::lex
{
    CERBERUS_ENUM(// NOLINTNEXTLINE
        ItemFlags, u16, NONE = 0b0, STAR = 0b1, PLUS = 0b10, QUESTION = 0b1'00, FIXED = 0b1'000,
        PREFIX = 0b10'000, REVERSE = 0b100'000, NONTERMINAL = 0b10'000'000);

    template<CharacterLiteral CharT>
    struct AnalysisGlobals
    {
        constexpr auto emplaceNonterminal(std::basic_string<CharT> &&str, size_t id)
            -> decltype(auto)
        {
            return nonterminals.emplace(std::move(str), id);
        }

        constexpr auto emplaceNonterminal(std::basic_string<CharT> const &str, size_t id)
            -> decltype(auto)
        {
            return nonterminals.emplace(str, id);
        }

        AnalysisGlobals() = default;

        StringPool<CharT, size_t, true> nonterminals{};
        LazyExecutor<std::any> lazy_executor{ 2 };
    };

    template<CharacterLiteral CharT>
    struct BasicItem
    {
        template<std::integral Int>
        CERBLIB_DECL static auto cast(Int value) -> CharT
        {
            return static_cast<CharT>(value);
        }

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
