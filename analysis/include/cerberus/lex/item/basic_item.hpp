#ifndef CERBERUS_BASIC_ITEM_HPP
#define CERBERUS_BASIC_ITEM_HPP

#include <cerberus/lex/generator_for_text.hpp>
#include <cerberus/lex/string_to_codes.hpp>
#include <cerberus/string_pool.hpp>

#ifndef CERBLIB_BASIC_ITEM_ACCESS
#    define CERBLIB_BASIC_ITEM_ACCESS(CharT)                                                       \
        using parent = BasicItem<CharT>;                                                           \
        using parent::analysis_globals;                                                            \
        using parent::cast;                                                                        \
        using parent::flags
#endif /* CERBLIB_BASIC_ITEM_ACCESS */

#ifndef CERBLIB_BASIC_ITEM_ARGS
#    define CERBLIB_BASIC_ITEM_ARGS AnalysisGlobals<CharT> &analysis_parameters
#endif /* CERBLIB_BASIC_ITEM_ARGS */

#ifndef CERBLIB_CONSTRUCT_BASIC_ITEM
#    define CERBLIB_CONSTRUCT_BASIC_ITEM parent(analysis_parameters)
#endif /* CERBLIB_CONSTRUCT_BASIC_ITEM */

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
        ItemFlags flags{ ItemFlags::NONE };
        AnalysisGlobals<CharT> &analysis_globals;
        // NOLINTEND
    };
}// namespace cerb::lex

#endif /* CERBERUS_BASIC_ITEM_HPP */
