#ifndef CERBERUS_INT_HPP
#define CERBERUS_INT_HPP

#include <cerberus/number.hpp>
#include <cerberus/string_view.hpp>
#include <string>

namespace cerb::fmt
{
    namespace private_
    {
        template<CharacterLiteral CharT, std::integral Int>
        struct IntConvert
        {
            CERBLIB_DECL auto moveConvertedNumber() -> std::basic_string<CharT>
            {
                return std::move(converted_number);
            }

            CERBLIB_DECL auto getConvertedNumber() const -> std::basic_string<CharT> const &
            {
                return converted_number;
            }

            IntConvert() = default;

            constexpr explicit IntConvert(Int number_to_convert) : number(number_to_convert)
            {
                convertNumber();
            }

        private:
            constexpr auto convertNumber() -> void
            {
                if constexpr (CharacterLiteral<Int>) {
                    converted_number = std::basic_string<CharT>{ cast(number) };
                } else {
                    addMinusIfNeed();
                    processZeroNumber();
                    fillBuffer();
                    copyBufferToResult();
                }
            }

            constexpr auto fillBuffer() -> void
            {
                constexpr int decimal_notation = 10;

                while (number != 0) {
                    character_buffer[--buffer_index] = cast('0' + number % decimal_notation);
                    number /= decimal_notation;
                }
            }

            constexpr auto copyBufferToResult() -> void
            {
                for (; buffer_index != digits_in_number; ++buffer_index) {
                    converted_number.push_back(character_buffer[buffer_index]);
                }
            }

            constexpr auto processZeroNumber() -> void
            {
                if (number == 0) {
                    converted_number.push_back(cast('0'));
                }
            }

            constexpr auto addMinusIfNeed() -> void
            {
                if (number < 0) {
                    converted_number.push_back(cast('-'));
                }

                number = abs(number);
            }

            template<std::integral U>
            static constexpr auto cast(U value) -> CharT
            {
                return static_cast<CharT>(value);
            }

            constexpr static auto digits_in_number = std::numeric_limits<Int>::digits10 + 1;

            std::basic_string<CharT> converted_number{};
            std::array<CharT, digits_in_number> character_buffer{};
            size_t buffer_index = digits_in_number;
            Int number{};
        };
    }// namespace private_

    template<CharacterLiteral CharT, std::integral Int>
    constexpr auto convert(Int number) -> std::basic_string<CharT>
    {
        using namespace private_;

        IntConvert<CharT, Int> converter{ number };
        return converter.moveConvertedNumber();
    }
}// namespace cerb::fmt

#endif /* CERBERUS_INT_HPP */
