#ifndef CERBERUS_DOT_ITEM_OBJECT_HPP
#define CERBERUS_DOT_ITEM_OBJECT_HPP

#include <cerberus/enum.hpp>
#include <cerberus/lexical/location.hpp>
#include <cerberus/string_view.hpp>
#include <cerberus/type.hpp>

namespace cerb::lex {
    template<CharacterLiteral CharT>
    struct DotItemObject
    {
        using str_view = BasicStringView<CharT>;
        using text_iterator = GetIteratorType<TextGenerator<CharT>>;

        CERBERUS_ENUM(Flags, u32, NONE = 0b0, REVERSE = 0b1, PREFIX_OR_POSTFIX = 0b10);
        CERBERUS_ENUM(Rule, u32, NONE = 0, STAR = 1, PLUS = 2, QUESTION = 3);

        constexpr virtual auto setRule(Rule rule) -> void = 0;
        CERBLIB_DECL virtual auto getIterator() const -> text_iterator = 0;

        constexpr DotItemObject() = default;
        constexpr virtual ~DotItemObject() = default;
    };
}// namespace cerb::lex

#endif /* CERBERUS_DOT_ITEM_OBJECT_HPP */
