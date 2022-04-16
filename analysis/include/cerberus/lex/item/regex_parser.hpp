#ifndef CERBERUS_REGEX_PARSER_HPP
#define CERBERUS_REGEX_PARSER_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/number.hpp>
#include <cerberus/reference_wrapper.hpp>
#include <cerberus/string_view.hpp>
#include <cerberus/type.hpp>

namespace cerb::lex::regex
{
    template<CharacterLiteral CharT>
    struct RegexParser
    {
        constexpr static size_t number_of_chars = pow2<size_t>(bitsizeof(CharT));
        using bitmap_t = ConstBitmap<1, number_of_chars>;

        RegexParser() = default;

        constexpr explicit RegexParser(
            BasicStringView<CharT> const &regex,
            ReferenceWrapper<bitmap_t>
                bitmap_for_chars)
          : available_chars(bitmap_for_chars)
        {}

    private:
        ReferenceWrapper<bitmap_t> available_chars{};
    };
}// namespace cerb::lex::regex

#endif /* CERBERUS_REGEX_PARSER_HPP */
