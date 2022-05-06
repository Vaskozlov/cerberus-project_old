#ifndef CERBERUS_BASIC_ITEM_HPP
#define CERBERUS_BASIC_ITEM_HPP

#include <cerberus/lex/generator_for_text.hpp>
#include <cerberus/lex/lexical_analysis_exception.hpp>
#include <cerberus/lex/string_to_codes.hpp>
#include <cerberus/string_pool.hpp>

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
        template<typename... Ts>
        constexpr auto emplaceNonterminal(Ts &&...args) -> decltype(auto)
        {
            return nonterminals.emplace(std::forward<Ts>(args)...);
        }

        AnalysisGlobals() = default;

        // NOLINTBEGIN
        StringPool<CharT, size_t, true> nonterminals{};
        // NOLINTEND
    };

    template<CharacterLiteral CharT>
    struct BasicItem
    {
        template<std::integral T>
        CERBLIB_DECL static auto cast(T value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        BasicItem() = default;

        constexpr virtual ~BasicItem() = default;
        constexpr explicit BasicItem(AnalysisGlobals<CharT> &analysis_parameters)
          : analysis_globals(analysis_parameters)
        {}

        // NOLINTBEGIN
        AnalysisGlobals<CharT> &analysis_globals;
        ItemFlags flags{ ItemFlags::NONE };
        // NOLINTEND
    };

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
