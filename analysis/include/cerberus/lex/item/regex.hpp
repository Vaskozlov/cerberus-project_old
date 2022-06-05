#ifndef CERBERUS_REGEX_HPP
#define CERBERUS_REGEX_HPP

#include <cerberus/lex/item/basic_item.hpp>
#include <cerberus/lex/item/regex_parser.hpp>

namespace cerb::lex::regex
{
    template<CharacterLiteral CharT>
    struct RegexItem : public BasicItem<CharT>
    {
        CERBLIB_BASIC_ITEM_ACCESS(CharT);

        CERBLIB_DECL auto scan() const -> ScanResult override
        {
            return ScanResult{};
        }

        constexpr RegexItem(CERBLIB_BASIC_ITEM_ARGS, text::GeneratorForText<CharT> &regex_rule)
          : CERBLIB_CONSTRUCT_BASIC_ITEM
        {
            parseRegex(regex_rule);
        }

    private:
        constexpr auto parseRegex(text::GeneratorForText<CharT> &regex_rule) -> void
        {
            RegexParser<CharT> regex_parser{ regex_rule, available_chars };
        }

        constexpr auto postInitializationSetup() -> void override
        {
            if (flags.isSet(ItemFlags::REVERSE)) {
                available_chars.reverseValues();
            }
        }

        Bitmap available_chars{};
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct RegexItem<char>;
    extern template struct RegexItem<char8_t>;
    extern template struct RegexItem<char16_t>;
#endif /* CERBERUS_HEADER_ONLY */

}// namespace cerb::lex::regex

#endif /* CERBERUS_REGEX_HPP */
