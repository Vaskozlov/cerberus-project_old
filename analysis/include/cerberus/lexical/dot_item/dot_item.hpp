#ifndef CERBERUS_DOT_ITEM_HPP
#define CERBERUS_DOT_ITEM_HPP

#include <cerberus/lexical/dot_item/dot_item_object.hpp>
#include <cerberus/lexical/sequence.hpp>
#include <cerberus/lexical/string_sequence.hpp>
#include <cerberus/lexical/tokens/token.hpp>

namespace cerb::lex {
    template<CharacterLiteral CharT, typename TokenType>
    struct DotItem
    {
        using str_view_t = BasicStringView<CharT>;

        using sequence_t = Sequence<CharT, TokenType>;
        using string_sequence_t = StringSequence<CharT, TokenType>;

        using Rule = typename sequence_t::Rule;
        using Flags = typename sequence_t::Flags;

        using object_t = DotItemObject<CharT, TokenType>;
        using parameters_pack_t = ParametersPack<CharT, TokenType>;

        using ScanStatus = typename object_t::ScanStatus;
        using generator_t = typename object_t::generator_t;
        using generator_reference_t = typename object_t::generator_reference_t;

        using storage_t = std::vector<std::unique_ptr<object_t>>;

        template<std::integral U>
        CERBLIB_DECL static auto cast(U value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        constexpr auto setInput(generator_t &&new_generator) -> void
        {
            generator_for_text = std::move(new_generator);
        }

        constexpr auto setInput(generator_t const &new_generator) -> void
        {
            generator_for_text = new_generator;
        }

        constexpr auto updateGeneratorFromPack() -> void
        {
            generator_for_text = analysis_parameters.generator_for_text;
        }

        constexpr auto scan() -> ScanStatus
        {
            for (auto &object : objects) {
                CERBLIB_IGNORE(object->scan())
            }

            return ScanStatus{};
        }

        explicit constexpr DotItem(
            str_view_t regex, parameters_pack_t const &parameters_for_analysis)
          : generator_for_text(LocationInFile{ "DotItem generator" }, regex),
            analysis_parameters(parameters_for_analysis)
        {
            parseRegex();
        }

    private:
        constexpr auto clearFlags()
        {
            flags_for_sequence = Flags::NONE;
        }

        CERBLIB_DECL auto getLastSequence() -> object_t &
        {
            if (objects.empty()) {
                throw ParsingError("Unable to find an sequences");
            }

            return *objects.back();
        }

        CERBLIB_DECL auto getLastSequence() const -> object_t &
        {
            if (objects.empty()) {
                throw ParsingError("Unable to find an sequences");
            }

            return *objects.back();
        }

        constexpr auto parseRegex()
        {
            for (CharT chr : generator_for_text) {
                switch (chr) {
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
                analysis_parameters, flags_for_sequence, ReferenceWrapper(generator_for_text));
            new_sequence->setGenerator(ReferenceWrapper(generator_for_text));

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
                analysis_parameters, flags_for_sequence, ReferenceWrapper(generator_for_text));
            new_string_sequence->setGenerator(ReferenceWrapper(generator_for_text));

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

        constexpr static auto generator_end = generator_t::end();

        generator_t generator_for_text;
        storage_t objects;
        parameters_pack_t const &analysis_parameters;
        Flags flags_for_sequence;
    };
}// namespace cerb::lex

#endif /* CERBERUS_DOT_ITEM_HPP */
