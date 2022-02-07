#ifndef CERBERUS_DOT_ITEM_HPP
#define CERBERUS_DOT_ITEM_HPP

#include <cerberus/lexical/dot_item/dot_item_object.hpp>
#include <cerberus/lexical/sequence.hpp>
#include <cerberus/lexical/string_sequence.hpp>
#include <cerberus/lexical/tokens/token.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT, typename TokenType>
    struct DotItem
    {
        using chars_enum_t = CharsEnum<CharT>;
        using str_view_t = BasicStringView<CharT>;
        using sequence_t = Sequence<CharT, TokenType>;
        using string_sequence_t = StringSequence<CharT, TokenType>;

        using token_t = Token<CharT, TokenType>;
        using object_t = DotItemObject<CharT, TokenType>;
        using storage_t = std::vector<std::unique_ptr<object_t>>;
        using parameters_pack_t = ParametersPack<CharT, TokenType>;
        using dot_item_pack_t = DotItemParameters<CharT, TokenType>;

        using generator_t = typename object_t::generator_t;

        using Rule = typename sequence_t::Rule;
        using Flags = typename sequence_t::Flags;
        using ScanStatus = typename object_t::ScanStatus;

        template<std::integral U>
        CERBLIB_DECL static auto cast(U value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        constexpr auto setInput(generator_t &&new_generator) -> void
        {
            getTextGenerator() = std::move(new_generator);
        }

        constexpr auto setInput(generator_t const &new_generator) -> void
        {
            getTextGenerator() = new_generator;
        }

        constexpr auto updateGeneratorFromPack() -> void
        {
            getTextGenerator() = analysis_parameters.generator_for_text;
        }

        constexpr auto scan() -> ScanStatus
        {
            for (auto &object : objects) {
                if (object->scan() == ScanStatus::ERROR) {
                    return ScanStatus::ERROR;
                }
            }

            return ScanStatus{};
        }

        explicit constexpr DotItem(
            str_view_t regex, parameters_pack_t const &parameters_for_analysis)
          : dot_item_pack(regex, "DotItem generator"), analysis_parameters(parameters_for_analysis)
        {
            parseRegex();
        }

    private:
        CERBLIB_DECL auto getRecognizedToken() -> token_t &
        {
            return dot_item_pack.recognized_token;
        }

        CERBLIB_DECL auto getRecognizedToken() const -> token_t &
        {
            return dot_item_pack.recognized_token;
        }

        CERBLIB_DECL auto getTextGenerator() -> generator_t &
        {
            return dot_item_pack.text_generator;
        }

        CERBLIB_DECL auto getTextGenerator() const -> generator_t &
        {
            return dot_item_pack.text_generator;
        }

        constexpr auto clearFlags()
        {
            flags_for_sequence = Flags::NONE;
        }

        CERBLIB_DECL auto getLastSequence() -> object_t &
        {
            if (not objects.empty()) [[likely]] {
                return *objects.back();
            }

            throw ParsingError("Unable to find an sequences");
        }

        CERBLIB_DECL auto getLastSequence() const -> object_t &
        {
            if (not objects.empty()) [[likely]] {
                return *objects.back();
            }

            throw ParsingError("Unable to find an sequences");
        }

        constexpr auto parseRegex()
        {
            generator_t &text_generator = getTextGenerator();

            while (text_generator.getCharWithoutLayout() != chars_enum_t::EoF) {
                switch (text_generator.getCurrentChar()) {
                case '[':
                    startNewSequenceOfChar();
                    endNewSequenceOfChar();
                    break;

                case 'p':
                    addPrefixOrPostfixTag();
                    break;

                case 'r':
                    addReverseTag();
                    break;

                case '"':
                    processString();
                    break;

                case '*':
                    addStarRule();
                    break;

                case '+':
                    addPlusRule();
                    break;

                case '?':
                    addQuestionRule();
                    break;

                default:
                    throw ParsingError("Unable to do anything with given regex");
                }
            }
        }

        constexpr auto startNewSequenceOfChar() -> void
        {
            std::unique_ptr<object_t> new_sequence = std::make_unique<sequence_t>(
                analysis_parameters, flags_for_sequence, ReferenceWrapper(dot_item_pack));

            objects.push_back(std::move(new_sequence));
        }

        constexpr auto endNewSequenceOfChar() -> void
        {
            clearFlags();
        }

        constexpr auto addPrefixOrPostfixTag() -> void
        {
            flags_for_sequence |= Flags::PREFIX_OR_POSTFIX;
        }

        constexpr auto addReverseTag() -> void
        {
            flags_for_sequence |= Flags::REVERSE;
        }

        constexpr auto processString() -> void
        {
            std::unique_ptr<object_t> new_string_sequence = std::make_unique<string_sequence_t>(
                analysis_parameters, flags_for_sequence, ReferenceWrapper(dot_item_pack));

            objects.push_back(std::move(new_string_sequence));
        }

        constexpr auto addStarRule() -> void
        {
            getLastSequence().setRule(Rule::STAR);
        }

        constexpr auto addPlusRule() -> void
        {
            getLastSequence().setRule(Rule::PLUS);
        }

        constexpr auto addQuestionRule() -> void
        {
            getLastSequence().setRule(Rule::QUESTION);
        }

        storage_t objects;
        dot_item_pack_t dot_item_pack{};
        parameters_pack_t const &analysis_parameters;
        Flags flags_for_sequence;
    };
}// namespace cerb::lex

#endif /* CERBERUS_DOT_ITEM_HPP */
