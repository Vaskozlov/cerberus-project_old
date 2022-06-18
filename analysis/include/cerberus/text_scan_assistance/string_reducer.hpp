#ifndef CERBERUS_STRING_REDUCER_HPP
#define CERBERUS_STRING_REDUCER_HPP

#include <cerberus/text_scan_assistance/generator_for_text.hpp>
#include <string>

namespace cerb::text
{
    template<CharacterLiteral CharT>
    struct StringReducer
    {
        CERBLIB_DECL auto getErrorPositionAfterReducing() const -> size_t
        {
            return base_index - left_border;
        }

        CERBLIB_DECL auto getReducedString() const -> BasicStringView<CharT> const &
        {
            return reduced_string;
        }

        StringReducer() = default;

        constexpr explicit StringReducer(GeneratorForText<CharT> const &generator)
          : text_generator(generator), base_index(generator.getOffset() - 1)
        {
            reduceString();
        }

    private:
        constexpr auto reduceString() -> void
        {
            reduceLeftBorder();
            reduceRightBorder();
            reduced_string = { line.begin() + left_border, right_border - left_border };
        }

        constexpr auto reduceLeftBorder() -> void
        {
            while (leftBorderReducingNeeded()) {
                --left_border;
            }
        }

        constexpr auto reduceRightBorder() -> void
        {
            while (rightBorderReducingNeeded()) {
                ++right_border;
            }
        }

        CERBLIB_DECL auto leftBorderReducingNeeded() const -> bool
        {
            constexpr size_t left_border_max_length = 20;
            size_t left_border_length = base_index - left_border;

            bool is_not_end_of_string = left_border > 0;
            bool in_border = left_border_length < left_border_max_length;
            bool is_not_layout_char = not lex::isLayout(line[left_border]);

            return (in_border || is_not_layout_char) && is_not_end_of_string;
        }

        CERBLIB_DECL auto rightBorderReducingNeeded() const -> bool
        {
            constexpr size_t right_border_max_length = 20;
            size_t right_border_length = right_border - base_index;

            bool is_not_end_of_string = right_border < line.size();
            bool in_border = right_border_length < right_border_max_length;
            bool is_not_layout_char = not lex::isLayout(line[right_border]);

            return (in_border || is_not_layout_char) && is_not_end_of_string;
        }

        BasicStringView<CharT> reduced_string{};
        GeneratorForText<CharT> const &text_generator;
        BasicStringView<CharT> const &line{ text_generator.getCurrentLine() };
        size_t base_index{};
        size_t left_border{ base_index };
        size_t right_border{ base_index };
    };
}// namespace cerb::text

#endif /* CERBERUS_STRING_REDUCER_HPP */
