#ifndef CERBERUS_FORMAT_HPP
#define CERBERUS_FORMAT_HPP

#include <cerberus/char.hpp>
#include <cerberus/format/convert/char_pointer.hpp>
#include <cerberus/format/convert/int.hpp>
#include <cerberus/format/convert/iterable.hpp>
#include <cerberus/format/convert/standart_containers.hpp>

namespace cerb::fmt
{
    CERBERUS_EXCEPTION(FmtConverterError, CerberusException);

    namespace private_
    {
        template<CharacterLiteral CharT>
        struct Converter
        {
            CERBLIB_DECL auto moveFormattedString() -> std::basic_string<CharT>
            {
                return std::move(formatted_string);
            }

            CERBLIB_DECL auto getFormattedString() const -> std::basic_string<CharT> const &
            {
                return formatted_string;
            }

            Converter() = default;

            template<typename... Ts>
            constexpr explicit Converter(string_view const &string_to_format, Ts &&...args)
              : formatter(string_to_format)
            {
                constexpr auto arg_count = sizeof...(Ts);
                constexpr auto average_arg_size = 16;
                constexpr auto extra_string_size = arg_count * average_arg_size;

                formatted_string.reserve(string_to_format.size() + extra_string_size);
                formatString(std::forward<Ts>(args)...);
            }

        private:
            template<std::integral Int>
            CERBLIB_DECL static auto cast(Int value) -> CharT
            {
                return static_cast<CharT>(value);
            }

            constexpr auto formatString() -> void
            {
                while (canContinueFormatting()) {
                    CharT chr = getChar();
                    processChar(chr);
                }
            }

            template<typename T, typename... Ts>
            constexpr auto formatString(T const &value, Ts &&...args) -> void
            {
                while (canContinueFormatting()) {
                    CharT chr = getChar();
                    processChar(chr);
                    convertValueIfNeed(value, std::forward<Ts>(args)...);
                }
            }

            constexpr auto processChar(CharT chr) -> void
            {
                switch (chr) {
                case cast('{'):
                    processOpeningBracket();
                    break;

                case cast('}'):
                    processClosingBracket();
                    break;

                default:
                    checkForOpenFmt();
                    appendChar(chr);
                }
            }

            constexpr auto processOpeningBracket() -> void
            {
                CharT chr = getFutureChar();

                if (chr == cast('{')) {
                    appendChar(chr);
                    nextChar();
                } else {
                    fmt_open = true;
                }
            }

            constexpr auto processClosingBracket() -> void
            {
                CharT chr = getFutureChar();

                if (logicalAnd(not fmt_open, chr == cast('}'))) {
                    appendChar(chr);
                    nextChar();
                } else {
                    fmt_open = false;
                    need_to_print_value = true;
                }
            }

            template<typename T, typename... Ts>
            constexpr auto convertValueIfNeed(T const &value, Ts &&...args) -> void
            {
                if (need_to_print_value) {
                    convertValue(value);
                    formatString(std::forward<Ts>(args)...);
                }
            }

            constexpr auto checkThatValueDoesNotNeedToBePrinted() const -> void
            {
                if (need_to_print_value) {
                    throw FmtConverterError("Too many arguments have been past to formatting");
                }
            }

            template<typename T>
            constexpr auto convertValue(T const &value) -> void
            {
                formatted_string += cerb::fmt::convert<CharT>(value);
                need_to_print_value = false;
            }

            constexpr auto canContinueFormatting() -> bool
            {
                CharT chr = nextChar();
                return not lex::isEoF(chr);
            }

            constexpr auto appendChar(CharT chr) -> void
            {
                formatted_string.push_back(chr);
            }

            CERBLIB_DECL auto getChar() -> CharT
            {
                if (not canAccessChar()) {
                    return lex::CharEnum<CharT>::EoF;
                }

                return cast(formatter[current_char_index]);
            }

            CERBLIB_DECL auto getFutureChar() const -> CharT
            {
                if (not canAccessChar(1)) {
                    return lex::CharEnum<CharT>::EoF;
                }

                return cast(formatter[current_char_index + 1]);
            }

            constexpr auto nextChar() -> CharT
            {
                if (not initialized) {
                    initialized = true;
                } else {
                    ++current_char_index;
                }

                return getChar();
            }

            CERBLIB_DECL auto canAccessChar(size_t offset = 0) const -> bool
            {
                return logicalAnd(initialized, current_char_index + offset < formatter.size());
            }

            constexpr auto checkForOpenFmt() const -> void
            {
                if (fmt_open) {
                    throw FmtConverterError(
                        "Arguments for cerb::fmt::format are not supported at the "
                        "moment!");
                }
            }

            std::basic_string<CharT> formatted_string{};
            string_view formatter{};
            size_t current_char_index{};
            bool fmt_open{};
            bool initialized{};
            bool need_to_print_value{};
        };
    }// namespace private_

    template<typename CharT, typename... Ts>
    CERBLIB_DECL auto format(string_view const &formatter, Ts &&...args) -> std::basic_string<CharT>
    {
        using namespace private_;

        Converter<CharT> converter{ formatter, std::forward<Ts>(args)... };
        return converter.moveFormattedString();
    }
}// namespace cerb::fmt

#endif /* CERBERUS_FORMAT_HPP */
