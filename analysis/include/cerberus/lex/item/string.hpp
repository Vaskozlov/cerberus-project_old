#ifndef CERBERUS_LEX_STRING_HPP
#define CERBERUS_LEX_STRING_HPP

#include <cerberus/lex/item/basic_item.hpp>


namespace cerb::lex::string
{
    template<CharacterLiteral CharT>
    struct StringItem : public BasicItem<CharT>
    {
        CERBLIB_BASIC_ITEM_ACCESS(CharT);

        CERBLIB_DECL auto getParsedStringLength() const -> size_t
        {
            return parsed_string_length;
        }

        constexpr explicit StringItem(CERBLIB_BASIC_ITEM_ARGS, BasicStringView<CharT> const &str)
          : CERBLIB_CONSTRUCT_BASIC_ITEM
        {
            StringToCodes<CharT> string_to_codes{ cast('\"'), str };
            string = std::move(string_to_codes.parseString());
            parsed_string_length = string_to_codes.getProcessedLength();
        }

    private:
        std::basic_string<CharT> string{};
        size_t parsed_string_length{};
        size_t index{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_LEX_STRING_HPP */
