#ifndef CERBERUS_LEXICAL_ANALYZER_HPP
#define CERBERUS_LEXICAL_ANALYZER_HPP

#include <cerberus/lexical/dot_item.hpp>
#include <cerberus/lexical/token.hpp>
#include <cerberus/string_pool.hpp>
#include <thread>

namespace cerb::lex {
    template<CharacterLiteral CharT, typename TokenType>
    class LexicalAnalyzer
    {
        using token_t = Token<CharT, TokenType>;
        using str_view = BasicStringView<CharT>;
        using string_pool = StringPool<CharT, TokenType>;
        using dot_item = DotItem<CharT, TokenType>;
        using ParameterPack = typename dot_item::ParametersPack;
        using CommentsTokens = typename dot_item::CommentsTokens;
        using text_generator = GeneratorForText<CharT>;
        using text_iterator = GetIteratorType<text_generator>;
        using string_parser = StringParser<CharT, text_iterator>;

        struct SimpleToken
        {
            CharT separator{};
            TokenType type{};
        };

        template<std::integral T>
        CERBLIB_DECL static auto cast(T value) -> CharT
        {
            return static_cast<CharT>(value);
        }

    public:
        CERBLIB_DECL auto yield() -> token_t
        {
            return scan();
        }

        constexpr auto setStream(str_view const &filename, str_view const &input) -> void
        {
            input_stream = text_generator(LocationInFile(filename), input);
        }

        constexpr LexicalAnalyzer(
            SimpleToken const &token_for_char, SimpleToken const &token_for_string,
            CommentsTokens const &tokens_of_comments,
            StringPool<CharT, TokenType> const &terminals_pool)
          : terminals(terminals_pool), comments_tokens(tokens_of_comments),
            char_token(token_for_char), string_token(token_for_string)
        {
            std::ranges::for_each(terminals_pool, [this](const auto &elem) {
                items_storage.emplace_back(elem.first);
            });
        }

    private:
        constexpr auto scan() -> token_t
        {
            skipLayoutAndComments();

            for (dot_item &item : items_storage) {
                item.setInput(std::move(input_stream.begin()));
                item.scan();
            }

            return token_t{};
        }

        constexpr auto skipLayoutAndComments() -> void
        {
            input_stream.skipLayout();

            while (hasSkippedComment()) {
                input_stream.skipLayout();
            }
        }

        constexpr auto hasSkippedComment() -> bool
        {
            str_view current_state = input_stream.getStrFromCurrentState();

            if (current_state.containsAt(0, comments_tokens.single_line_begin)) {
                skipSingleLineComment();
                return true;
            }
            if (current_state.containsAt(0, comments_tokens.multiline_begin)) {
                skipMultilineComment(current_state);
                return true;
            }

            return false;
        }

        constexpr auto skipSingleLineComment() -> void
        {
            CharT chr = input_stream.newRawChar();

            while (logicalAnd(chr != cast('\n'), isEndOfInput(chr))) {
                chr = input_stream.newRawChar();
            }
        }

        constexpr auto skipMultilineComment(str_view const &current_state) -> void
        {
            size_t index = 0;

            for (; index < comments_tokens.multiline_begin.size(); ++index) {
                throwIfEoF(input_stream.newRawChar());
            }

            while (!current_state.containsAt(index, comments_tokens.multiline_end)) {
                throwIfEoF(input_stream.newRawChar());
                ++index;
            }

            for (index = 0; index < comments_tokens.multiline_end.size(); ++index) {
                throwIfEoF(input_stream.newRawChar());
            }
        }

        constexpr auto throwIfEoF(CharT chr) const -> void
        {
            if (isEndOfInput(chr)) {
                throw std::invalid_argument("Unexpected end of input on given string.");
            }
        }

        ParameterPack lexical_analysis_parameters{};
        std::vector<dot_item> items_storage{};
        string_pool terminals{};
        text_generator input_stream{};
        CommentsTokens comments_tokens{};
        SimpleToken char_token{};
        SimpleToken string_token{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_LEXICAL_ANALYZER_HPP */
