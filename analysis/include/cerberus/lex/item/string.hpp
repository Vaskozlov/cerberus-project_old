#ifndef CERBERUS_LEX_STRING_HPP
#define CERBERUS_LEX_STRING_HPP

#include <cerberus/lex/item/basic_item.hpp>


namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct StringItem : public BasicItem<CharT>
    {
        CERBLIB_BASIC_ITEM_ACCESS(CharT);

        constexpr auto parseString(BasicStringView<CharT> const &str) -> size_t
        {
            StringToCodes<CharT> string_to_codes{ cast('\"'), str };
            string = std::move(string_to_codes.parseString());

            return string_to_codes.getProcessedLength();
        }

    private:
        std::basic_string<CharT> string{};
        size_t index{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_LEX_STRING_HPP */
