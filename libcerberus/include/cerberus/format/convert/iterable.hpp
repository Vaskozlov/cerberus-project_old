#ifndef CERBERUS_ITERABLE_HPP
#define CERBERUS_ITERABLE_HPP

#include <cerberus/string_view.hpp>
#include <string>

namespace cerb::fmt
{
    namespace private_
    {
        template<CharacterLiteral CharT, Iterable T>
        struct IterableConverter
        {
            CERBLIB_DECL auto moveResult() -> std::basic_string<CharT>
            {
                return std::move(conversion_result);
            }

            CERBLIB_DECL auto getResult() const -> std::basic_string<CharT> const &
            {
                return conversion_result;
            }

            IterableConverter() = default;

            constexpr IterableConverter(
                char begin_for_object, char end_for_object, T const &object_to_convert)
              : converted_object(object_to_convert), object_begin(cast(begin_for_object)),
                object_end(cast(end_for_object))
            {
                convertObject();
            }

        private:
            constexpr auto convertObject() -> void
            {
                addBeginOfObject();

                for (auto const &elem : converted_object) {
                    addElem(elem);
                }

                addEndOfObject();
            }

            constexpr auto addElem(auto const &elem) -> void
            {
                conversion_result += convert<CharT>(elem);
                addElemSeparator();
            }

            constexpr auto addElemSeparator() -> void
            {
                conversion_result.push_back(cast(','));
                conversion_result.push_back(cast(' '));
            }

            constexpr auto addBeginOfObject() -> void
            {
                conversion_result.push_back(object_begin);
            }

            constexpr auto addEndOfObject() -> void
            {
                if (std::size(converted_object) != 0) {
                    conversion_result.pop_back();
                    conversion_result.back() = object_end;
                } else {
                    conversion_result.push_back(object_end);
                }
            }

            template<std::integral U>
            static constexpr auto cast(U value) -> CharT
            {
                return static_cast<CharT>(value);
            }

            std::basic_string<CharT> conversion_result{};
            T const &converted_object;
            CharT object_begin{};
            CharT object_end{};
        };

        template<CharacterLiteral CharT, Iterable T>
        constexpr auto// NOLINTNEXTLINE
            convertIterable(char object_begin, char object_end, T const &object_to_convert)
                -> std::basic_string<CharT>
        {
            IterableConverter<CharT, T> converter{ object_begin, object_end, object_to_convert };
            return converter.moveResult();
        }

    }// namespace private_

    template<CharacterLiteral CharT, Iterable T>
    constexpr auto convert(T const &iterable_obj) -> std::basic_string<CharT>
    {
        std::basic_string<CharT> result{};

        for (auto const &elem : iterable_obj) {
            result += convert<CharT>(elem);
        }

        return result;
    }
}// namespace cerb::fmt

#endif /* CERBERUS_ITERABLE_HPP */
