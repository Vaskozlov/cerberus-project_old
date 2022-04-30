#ifndef CERBERUS_INT_HPP
#define CERBERUS_INT_HPP

#include <cerberus/number.hpp>
#include <cerberus/string_view.hpp>
#include <string>

#define CCHAR(x) static_cast<CharT>(x)

namespace cerb::fmt
{
    namespace private_
    {
        template<CharacterLiteral CharT, std::integral T>
        struct IntConvert
        {
            CERBLIB_DECL auto moveResult() -> std::basic_string<CharT>
            {
                return std::move(converted_number);
            }

            CERBLIB_DECL auto getResult() const -> std::basic_string<CharT> const &
            {
                return converted_number;
            }

            IntConvert() = default;

            constexpr explicit IntConvert(T number_to_convert) : number(number_to_convert)
            {
                convertNumber();
            }

        private:
            constexpr auto convertNumber() -> void
            {
                if constexpr (CharacterLiteral<T>) {
                    converted_number = std::basic_string<CharT>{ cast(number) };
                    return;
                }

                addMinusIfNeed();
                processZeroNumber();
                fillBuffer();
                copyBufferToResult();
            }

            constexpr auto fillBuffer() -> void
            {
                while (number != 0) {
                    character_buffer[--buffer_index] = cast('0' + number % 10);
                    number /= 10;
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

            constexpr static auto digits_in_number = std::numeric_limits<T>::digits10 + 1;

            std::basic_string<CharT> converted_number{};
            std::array<CharT, digits_in_number> character_buffer{};
            size_t buffer_index = digits_in_number;
            T number{};
        };
    }// namespace private_

    template<CharacterLiteral CharT, std::integral T>
    constexpr auto convert(T number) -> std::basic_string<CharT>
    {
        using namespace private_;

        IntConvert<CharT, T> converter{ number };
        return converter.moveResult();
    }
}// namespace cerb::fmt

#undef CCHAR

#endif /* CERBERUS_INT_HPP */
