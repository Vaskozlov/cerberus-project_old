#ifndef CERBERUS_STRING_REDUCER_HPP
#define CERBERUS_STRING_REDUCER_HPP

#include <cerberus/string_view.hpp>
#include <string>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct StringReducer
    {
        CERBLIB_DECL auto getErrorPositionAfterReducing() const -> size_t
        {
            return error_index - left_error_border;
        }

        CERBLIB_DECL auto getResult() const -> BasicStringView<CharT> const &
        {
            return reduced_string;
        }

        StringReducer() = default;

        constexpr explicit StringReducer(GeneratorForText<CharT> const &generator)
          : text_generator(generator), error_index(generator.getCharPosition() - 1)
        {
            reduceString();
        }

    private:
        constexpr auto reduceString() -> void
        {
            reduceLeftBorder();
            reduceRightBorder();
            reduced_string = { line.begin() + left_error_border,
                               right_error_border - left_error_border };
        }

        constexpr auto reduceLeftBorder() -> void
        {
            while (leftBorderReducingNeeded()) {
                --left_error_border;
            }
        }

        constexpr auto reduceRightBorder() -> void
        {
            while (rightBorderReducingNeeded()) {
                ++right_error_border;
            }
        }

        CERBLIB_DECL auto leftBorderReducingNeeded() const -> bool
        {
            constexpr size_t left_border_max_length = 20;
            size_t left_border_length = error_index - left_error_border;

            bool is_not_end_of_string = left_error_border > 0;
            bool in_border = left_border_length < left_border_max_length;
            bool is_not_layout_char = not isLayout(line[left_error_border]);

            return (in_border || is_not_layout_char) && is_not_end_of_string;
        }

        CERBLIB_DECL auto rightBorderReducingNeeded() const -> bool
        {
            constexpr size_t right_border_max_length = 20;
            size_t right_border_length = right_error_border - error_index;

            bool is_not_end_of_string = right_error_border < line.size();
            bool in_border = right_border_length < right_border_max_length;
            bool is_not_layout_char = not isLayout(line[right_error_border]);

            return (in_border || is_not_layout_char) && is_not_end_of_string;
        }

        BasicStringView<CharT> reduced_string{};
        GeneratorForText<CharT> const &text_generator;
        BasicStringView<CharT> const &line{ text_generator.getCurrentLine() };
        size_t error_index{};
        size_t left_error_border{ error_index };
        size_t right_error_border{ error_index };
    };
}// namespace cerb::lex

#endif /* CERBERUS_STRING_REDUCER_HPP */
