#ifndef LIBCERBERUS_TYPE_HPP
#define LIBCERBERUS_TYPE_HPP

#include <memory>
#include <type_traits>

namespace cerb
{
    template<typename TargetType, typename... SuitableTypes>
    struct IsAnyOf
    {
        template<typename CurrentType2Check, typename... OtherTypes2Check>
        CERBLIB_DECL static auto check(bool result = false) -> bool
        {
            result = logicalOr(result, std::is_same_v<TargetType, CurrentType2Check>);

            if constexpr (sizeof...(OtherTypes2Check) != 0) {
                result = check<OtherTypes2Check...>(result);
            }

            return result;
        }

        static constexpr auto value = check<SuitableTypes...>();
    };

    template<typename T, typename... Ts>
    CERBLIB_DECL auto isOneOf(T target_value, Ts &&...suitable_values) -> bool
    {
        bool result = false;

        forEach(
            [&result, &target_value]<typename U>(const U &value) {
                if constexpr (std::convertible_to<U, T>) {
                    result = logicalOr(result, static_cast<T>(value) == target_value);
                }
            },
            std::forward<Ts>(suitable_values)...);

        return result;
    }

    template<typename TargetType, typename... OtherTypes>
    constexpr bool IsAnyOfV = IsAnyOf<TargetType, OtherTypes...>::value;

    template<typename T>
    concept Pairable = requires(T value)
    {
        value.first;
        value.second;
    };

    template<typename T>
    concept DataAccessible = requires(T value)
    {
        value.getData();
        value.size();
    };

    template<typename T>
    concept Iterable = requires(T value)
    {
        value.begin() != value.end();
    };

    template<typename T>
    concept HasGotValueType = requires(T value)
    {
        typename T::value_type;
    };

    template<typename T>
    concept HasGotSizeType = requires(T value)
    {
        typename T::size_type;
    };

    template<typename T>
    concept LessComparable = requires(T lhs, T rhs)
    {
        lhs < rhs;
    };

    template<typename T>
    concept Enum = std::is_enum_v<T>;

    template<typename T>
    concept CanBeStoredInIntegral = isOneOf(
        sizeof(T),
        sizeof(uint8_t),
        sizeof(uint16_t),
        sizeof(uint32_t),
        sizeof(uint64_t),
        sizeof(size_t));

    template<typename T>
    concept Trivial = std::is_trivial_v<T> &&(sizeof(T) <= sizeof(size_t) * 2);

    template<typename T>
    concept NotTrivial = !Trivial<T>;

    template<typename T>
    concept RawAccessible =
        Iterable<T> && DataAccessible<T> && std::contiguous_iterator<typename T::iterator>;

    template<typename T>
    concept ClassValueFastCopiable = std::is_trivially_copy_assignable_v<typename T::value_type>;

    template<typename CharT>
    concept CharacterLiteral =
        cerb::IsAnyOfV<CharT, char, unsigned char, char8_t, char16_t, char32_t, wchar_t>;

    template<typename T>
    using AutoCopyType = std::conditional_t<Trivial<T>, T const, T const &>;

    template<HasGotValueType T>
    using GetValueType = typename T::value_type;

    template<Iterable T>
    using GetIteratorType = typename T::iterator;

    template<HasGotSizeType T>
    using GetSizeType = typename T::size_type;

    template<typename T>
    concept TriviallyCopiable = std::is_same_v<T const, AutoCopyType<T const>>;

    template<typename T>
    concept NotTriviallyCopiable = not TriviallyCopiable<T>;
}// namespace cerb

#endif /* LIBCERBERUS_TYPE_HPP */
