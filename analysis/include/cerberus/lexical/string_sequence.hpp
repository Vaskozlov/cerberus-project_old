#ifndef CERBERUS_STRING_SEQUENCE_HPP
#define CERBERUS_STRING_SEQUENCE_HPP

#include <cerberus/lexical/dot_item/dot_item_object.hpp>
#include <cerberus/lexical/string_parser.hpp>
#include <string>

namespace cerb::lex {
    template<CharacterLiteral CharT, typename TokenType>
    struct StringSequence final : public DotItemObject<CharT, TokenType>
    {
        using str_t = std::basic_string<CharT>;
        using parent = DotItemObject<CharT, TokenType>;

        using parent::is_prefix_or_postfix;
        using parent::text_generator;

        using Flags = typename parent::Flags;
        using ScanStatus = typename parent::ScanStatus;

        using generator_t = typename parent::generator_t;
        using string_parser = StringParser<CharT, generator_t>;
        using parameters_pack_t = typename parent::parameters_pack_t;
        using generator_reference_t = typename parent::generator_reference_t;

        constexpr auto scan() -> ScanStatus override
        {
            for (CharT chr : parsed_string) {
                if (not scanChar(chr)) {
                    return ScanStatus::ERROR;
                }
            }

            return ScanStatus::REDUCED;
        }

        constexpr StringSequence(
            parameters_pack_t const &parameters_for_analysis, Flags object_flags,
            generator_reference_t text_generator)
          : parent(reference(text_generator), parameters_for_analysis), flags(object_flags)
        {
            string_parser parser_for_string{ static_cast<CharT>('\"'), text_generator.get() };
            parsed_string = std::move(parser_for_string.parseString());

            if (flags.isSet(Flags::PREFIX_OR_POSTFIX)) {
                is_prefix_or_postfix = true;
            }
        }

    private:
        CERBLIB_DECL auto scanChar(CharT chr) const -> bool
        {
            if (flags.isSet(Flags::REVERSE)) {
                return reversedCharScan(chr);
            }

            return ordinaryCharScan(chr);
        }

        CERBLIB_DECL auto reversedCharScan(CharT chr) const -> bool
        {
            return not(chr == text_generator.getCharAtCurrentOffset());
        }

        CERBLIB_DECL auto ordinaryCharScan(CharT chr) const -> bool
        {
            return chr == text_generator.getCharAtCurrentOffset();
        }

        str_t parsed_string{};
        Flags flags{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_STRING_SEQUENCE_HPP */
