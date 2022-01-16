#ifndef CERBERUS_LEXICAL_ANALYZER_HPP
#define CERBERUS_LEXICAL_ANALYZER_HPP

#include <cerberus/lexical/dot_item/dot_item.hpp>
#include <cerberus/lexical/tokens/token.hpp>
#include <future>
#include <thread>

namespace cerb::lex
{
    template<CharacterLiteral CharT, typename TokenType>
    class LexicalAnalyzer
    {
        using token_t = Token<CharT, TokenType>;
        using str_view_t = BasicStringView<CharT>;
        using dot_item_t = DotItem<CharT, TokenType>;
        using simple_token_t = SimpleToken<CharT, TokenType>;
        using comments_token_t = CommentsTokens<CharT>;

        using generator_t = typename dot_item_t::generator_t;
        using parameters_pack_t = typename dot_item_t::parameters_pack_t;

        template<std::integral T>
        CERBLIB_DECL static auto cast(T value) -> CharT
        {
            return static_cast<CharT>(value);
        }

        CERBLIB_DECL auto getSingleLineCommentBegin() const -> str_view_t const &
        {
            return analysis_parameters.getSingleLineCommentBegin();
        }

        CERBLIB_DECL auto getMultilineCommentBegin() const -> str_view_t const &
        {
            return analysis_parameters.getMultilineCommentBegin();
        }

        CERBLIB_DECL auto getMultilineCommentEnd() const -> str_view_t const &
        {
            return analysis_parameters.getMultilineCommentEnd();
        }

    public:
        CERBLIB_DECL auto yield() -> token_t
        {
            return scan();
        }

        constexpr auto setStream(str_view_t const &filename, str_view_t const &input) -> void
        {
            text_generator = { LocationInFile(filename), input };
        }

        LexicalAnalyzer(
            simple_token_t const &token_for_char, simple_token_t const &token_for_string,
            comments_token_t const &tokens_of_comments,
            std::initializer_list<const Pair<str_view_t, TokenType>> const &items_rules,
            StringPool<CharT, TokenType> const &terminals_pool)
          : analysis_parameters(
                terminals_pool, tokens_of_comments, {}, token_for_char, token_for_string)
        {
            std::vector<std::future<void>> processed_items{};

            processed_items.reserve(items_rules.size());
            items_storage.reserve(items_rules.size());

            for (const auto &elem : items_rules) {
                processed_items.emplace_back(std::async(std::launch::async, [&elem, this]() {
                    items_storage.emplace_back(elem.first, analysis_parameters);
                }));
            }

            for (std::future<void> &elem : processed_items) {
                elem.get();
            }
        }

    private:
        constexpr auto scan() -> token_t
        {
            skipLayoutAndComments();

            for (dot_item_t &item : items_storage) {
                item.setInput(text_generator);
                item.scan();
            }

            return token_t{};
        }

        constexpr auto skipLayoutAndComments() -> void
        {
            text_generator.skipLayout();

            while (hasSkippedComment()) {
                text_generator.skipLayout();
            }
        }

        constexpr auto hasSkippedComment() -> bool
        {
            str_view_t current_state = text_generator.getCurrentStateString();

            if (current_state.containsAt(0, getSingleLineCommentBegin())) {
                skipSingleLineComment();
                return true;
            }
            if (current_state.containsAt(0, getMultilineCommentBegin())) {
                skipMultilineComment(current_state);
                return true;
            }

            return false;
        }

        constexpr auto skipSingleLineComment() -> void
        {
            CharT chr = text_generator.newRawChar();

            while (logicalAnd(chr != cast('\n'), isEndOfInput(chr))) {
                chr = text_generator.newRawChar();
            }
        }

        constexpr auto skipMultilineComment(str_view_t const &current_state) -> void
        {
            size_t index = 0;

            for (; index < getMultilineCommentBegin().size(); ++index) {
                throwIfEoF(text_generator.newRawChar());
            }

            while (!current_state.containsAt(index, getMultilineCommentEnd())) {
                throwIfEoF(text_generator.newRawChar());
                ++index;
            }

            for (index = 0; index < getMultilineCommentEnd().size(); ++index) {
                throwIfEoF(text_generator.newRawChar());
            }
        }

        constexpr auto throwIfEoF(CharT chr) const -> void
        {
            if (isEndOfInput(chr)) {
                throw std::invalid_argument("Unexpected end of input on given string.");
            }
        }

        parameters_pack_t analysis_parameters{};
        std::vector<dot_item_t> items_storage{};
        generator_t text_generator{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_LEXICAL_ANALYZER_HPP */
