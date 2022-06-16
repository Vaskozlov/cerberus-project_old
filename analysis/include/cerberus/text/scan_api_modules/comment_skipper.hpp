#ifndef CERBERUS_COMMENT_SKIPPER_HPP
#define CERBERUS_COMMENT_SKIPPER_HPP

#include <cerberus/analysis/analysis_exception.hpp>
#include <cerberus/text/generator_for_text.hpp>
#include <cerberus/text/scan_api_modules/skip_mode.hpp>

namespace cerb::text
{
    CERBERUS_EXCEPTION(BasicCommentSkipperException, analysis::BasicAnalysisException);

    template<CharacterLiteral CharT>
    CERBERUS_ANALYSIS_EXCEPTION(CommentSkipperException, CharT, BasicCommentSkipperException);

    template<CharacterLiteral CharT>
    struct CommentSkipper
    {
        using char_enum = lex::CharEnum<CharT>;
        friend CommentSkipperException<CharT>;

        constexpr auto skipComment() -> void
        {
            auto const &text = text_generator.getRestOfTheText();

            if (not single_line.empty() && isSingleLineComment(text)) {
                skipSingleLine();
            } else if (not multiline_begin.empty() && isMultilineComment(text)) {
                skipMultiline(text);
            }
        }

        CommentSkipper() = default;

        constexpr explicit CommentSkipper(
            GeneratorForText<CharT> &generator_for_text,
            BasicStringView<CharT> const &single_line_comment = {},
            BasicStringView<CharT> const &multiline_comment_begin = {},
            BasicStringView<CharT> const &multiline_comment_end = {})
          : single_line(single_line_comment), multiline_begin(multiline_comment_begin),
            multiline_end(multiline_comment_end), text_generator(generator_for_text)
        {}

    private:
        CERBLIB_DECL auto isSingleLineComment(BasicStringView<CharT> const &text) const -> bool
        {
            return text.containsAt(0, single_line);
        }

        CERBLIB_DECL auto isMultilineComment(BasicStringView<CharT> const &text) const -> bool
        {
            return text.containsAt(0, multiline_begin);
        }

        constexpr auto skipSingleLine() -> void
        {
            text_generator.skip(single_line.size());

            while (isNewLineOrEoF(text_generator.getRawChar())) {
                // empty loop
            }
        }

        constexpr auto skipMultiline(BasicStringView<CharT> const &text) -> void
        {
            size_t offset = multiline_begin.size();
            GeneratorForText<CharT> comment_begin = text_generator;

            text_generator.skip(offset);

            while (not text.containsAt(offset, multiline_end)) {
                ++offset;
            }

            if (not text.containsAt(offset, multiline_end)) {
                throw CommentSkipperException<CharT>("Unterminated comment.", comment_begin);
            }
        }

        CERBLIB_DECL static auto isNewLineOrEoF(CharT chr) -> bool
        {
            return logicalOr(chr == char_enum::NewLine, chr == char_enum::EoF);
        }

        BasicStringView<CharT> single_line{};
        BasicStringView<CharT> multiline_begin{};
        BasicStringView<CharT> multiline_end{};
        GeneratorForText<CharT> &text_generator;
    };
}// namespace cerb::text

#endif /* CERBERUS_COMMENT_SKIPPER_HPP */
