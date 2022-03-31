#ifndef CERBERUS_DOT_ITEM_OBJECT_HPP
#define CERBERUS_DOT_ITEM_OBJECT_HPP

#include <cerberus/enum.hpp>
#include <cerberus/lexical/dot_item/dot_item_pack.hpp>
#include <cerberus/lexical/dot_item/param_pack.hpp>
#include <cerberus/lexical/lexical_exceptions.hpp>
#include <cerberus/lexical/tokens/token.hpp>
#include <cerberus/reference_wrapper.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT, LessComparable TokenType>
    struct DotItemObject
    {
        using token_t = Token<CharT, TokenType>;
        using str_view_t = BasicStringView<CharT>;

        using generator_t = TextGenerator<CharT>;
        using string_pool_t = StringPool<CharT, TokenType>;

        using parameters_pack_t = ParametersPack<CharT, TokenType>;
        using dot_item_pack_t = DotItemParameters<CharT, TokenType>;

        CERBERUS_ENUM(ScanStatus, u16, SUCCESS = 0b1, ERROR = 0b10);                     // NOLINT
        CERBERUS_ENUM(Rule, u16, NONE = 0, STAR = 1, PLUS = 2, QUESTION = 3);            // NOLINT
        CERBERUS_ENUM(Flags, u16, NONE = 0b1, REVERSE = 0b10, PREFIX_OR_POSTFIX = 0b100);// NOLINT

        constexpr virtual auto scan() -> ScanStatus = 0;

        constexpr auto setRule(Rule rule) -> void
        {
            if (sequence_rule != Rule::NONE) {
                throw SequenceError("Attempt to set rule for sequence twice!");
            }
            sequence_rule = rule;
        }

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

        CERBLIB_DECL auto canGetNextChar() -> ScanStatus
        {
            generator_t &text_generator = getTextGenerator();
            str_view_t current_state = text_generator.getCurrentString();

            if (isComment(current_state)) {
                return ScanStatus::ERROR;
            }

            return ScanStatus::SUCCESS;
        }

        DotItemObject() = default;
        constexpr virtual ~DotItemObject() = default;

        constexpr explicit DotItemObject(
            dot_item_pack_t &dot_item_parameters,
            parameters_pack_t const &parameters_for_analysis)
          : dot_item_pack(dot_item_parameters), analysis_parameters(parameters_for_analysis)
        {}

    private:
        CERBLIB_DECL auto isComment(str_view_t const &current_state) -> bool
        {
            return isSingleLineComment(current_state) || isMultilineCommentBegin(current_state) ||
                   isMultilineCommentEnd(current_state);
        }

        CERBLIB_DECL auto isSingleLineComment(str_view_t const &current_state) -> bool
        {
            return current_state.containsAt(0, analysis_parameters.getSingleLineCommentBegin());
        }

        CERBLIB_DECL auto isMultilineCommentBegin(str_view_t const &current_state) -> bool
        {
            return current_state.containsAt(0, analysis_parameters.getMultilineCommentBegin());
        }

        CERBLIB_DECL auto isMultilineCommentEnd(str_view_t const &current_state) -> bool
        {
            return current_state.containsAt(0, analysis_parameters.getMultilineCommentEnd());
        }

        CERBLIB_DECL auto isTerminalSymbol(str_view_t const &current_state) -> bool
        {
            auto repr_of_terminal = analysis_parameters.findLongestStringInPool(current_state);
            return not repr_of_terminal.empty();
        }

        constexpr auto constructUnexpectedToken(TokenType token_type, str_view_t const &)
            -> void
        {
            getRecognizedToken() = token_t{
                token_type,
            };
        }

    protected:// NOLINTBEGIN
        dot_item_pack_t &dot_item_pack;
        parameters_pack_t const &analysis_parameters;
        Rule sequence_rule{ Rule::NONE };
        bool is_prefix_or_postfix{ false };

        // NOLINTEND
    };
}// namespace cerb::lex

#endif /* CERBERUS_DOT_ITEM_OBJECT_HPP */
