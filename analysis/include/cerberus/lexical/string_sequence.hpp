#ifndef CERBERUS_STRING_SEQUENCE_HPP
#define CERBERUS_STRING_SEQUENCE_HPP

#include <cerberus/lexical/dot_item/dot_item_object.hpp>
#include <cerberus/lexical/string_parser.hpp>
#include <string>

namespace cerb::lex
{
    template<CharacterLiteral CharT, typename TokenType>
    struct StringSequence final : public DotItemObject<CharT, TokenType>
    {
        using str_t = std::basic_string<CharT>;
        using string_parser = StringParser<CharT>;
        using parent = DotItemObject<CharT, TokenType>;

        using generator_t = typename parent::generator_t;
        using dot_item_pack_t = typename parent::dot_item_pack_t;
        using parameters_pack_t = typename parent::parameters_pack_t;

        using Flags = typename parent::Flags;
        using ScanStatus = typename parent::ScanStatus;

        using parent::dot_item_pack;
        using parent::getRecognizedToken;
        using parent::getTextGenerator;
        using parent::is_prefix_or_postfix;

        constexpr auto scan() -> ScanStatus override
        {
            for (CharT chr : parsed_string) {
                if (not scanChar(chr)) {
                    return ScanStatus::ERROR;
                }
            }

            return ScanStatus::SUCCESS;
        }

        constexpr StringSequence(
            parameters_pack_t const &parameters_for_analysis, Flags object_flags,
            dot_item_pack_t &dot_item_parameters)
          : parent(dot_item_parameters, parameters_for_analysis), flags(object_flags)
        {
            generator_t &text_generator = dot_item_parameters.text_generator;
            string_parser parser_for_string{ CharsEnum<CharT>::DQM, ref(text_generator) };
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
            generator_t &text_generator = getTextGenerator();
            return chr != text_generator.getCurrentChar();
        }

        CERBLIB_DECL auto ordinaryCharScan(CharT chr) const -> bool
        {
            generator_t &text_generator = getTextGenerator();
            return chr == text_generator.getCurrentChar();
        }

        str_t parsed_string{};
        Flags flags{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_STRING_SEQUENCE_HPP */
