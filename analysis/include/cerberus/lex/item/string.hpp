#ifndef CERBERUS_LEX_STRING_HPP
#define CERBERUS_LEX_STRING_HPP

#include <cerberus/lex/item/basic_item.hpp>

namespace cerb::lex::string
{
    template<CharacterLiteral CharT>
    struct StringItem : public BasicItem<CharT>
    {
        CERBLIB_BASIC_ITEM_ACCESS(CharT);
        CERBERUS_ANALYSIS_EXCEPTION(StringItemError, CharT, BasicLexicalAnalysisException);

        CERBLIB_DECL auto getString() const -> std::basic_string<CharT> const &
        {
            return string;
        }

        constexpr StringItem(CERBLIB_BASIC_ITEM_ARGS, GeneratorForText<CharT> &generator)
          : CERBLIB_CONSTRUCT_BASIC_ITEM, string(convertStringToCodes(cast('\"'), generator))
        {
            checkThatStringIsNotEmpty(generator);
        }

    private:
        constexpr auto checkThatStringIsNotEmpty(GeneratorForText<CharT> const &generator) const
            -> void
        {
            if (string.empty()) {
                throw StringItemError("Empty strings are not allowed!", generator);
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
