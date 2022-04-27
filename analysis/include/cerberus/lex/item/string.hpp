#ifndef CERBERUS_LEX_STRING_HPP
#define CERBERUS_LEX_STRING_HPP

#include <cerberus/lex/item/basic_item.hpp>

namespace cerb::lex::string
{
    CERBERUS_EXCEPTION(StringItemError);

    template<CharacterLiteral CharT>
    struct StringItem : public BasicItem<CharT>
    {
        CERBLIB_BASIC_ITEM_ACCESS(CharT);

        constexpr StringItem(CERBLIB_BASIC_ITEM_ARGS, GeneratorForText<CharT> &str)
          : CERBLIB_CONSTRUCT_BASIC_ITEM, string(convertStringToCodes(cast('\"'), str))
        {
            checkThatStringIsNotEmpty();
        }

    private:
        constexpr auto checkThatStringIsNotEmpty() const -> void
        {
            if (string.empty()) {
                throw StringItemError("Empty strings are not allowed!");
            }
        }

        std::basic_string<CharT> string{};
        size_t index{};
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct StringItem<char>;
    extern template struct StringItem<char8_t>;
    extern template struct StringItem<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::lex::string

#endif /* CERBERUS_LEX_STRING_HPP */
