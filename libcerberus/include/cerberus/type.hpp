#ifndef LIBCERBERUS_TYPE_HPP
#define LIBCERBERUS_TYPE_HPP

#include <memory>
#include <type_traits>

namespace cerb {
    template<typename TargetType, typename... SuitableTypes>
    struct is_any_of
    {
        template<typename CurrentType2Check, typename... OtherTypes2Check>
        CERBLIB_DECL static auto check(bool result = false) -> bool
        {
            result = result || std::is_same_v<TargetType, CurrentType2Check>;

            if constexpr (sizeof...(OtherTypes2Check) != 0) {
                result = check<OtherTypes2Check...>(result);
            }

            return result;
        }

        static constexpr auto value = check<SuitableTypes...>();
    };

    template<typename T, typename... Ts>
    constexpr auto is_one_of(T target_value, Ts &&...suitable_values) -> bool
    {
        bool result = false;

        for_each(
            [&result, &target_value]<typename U>(const U &value) {
                if constexpr (std::convertible_to<U, T>) {
                    result = result || (static_cast<T>(value) == target_value);
                }
            },
            std::forward<Ts>(suitable_values)...);

        return result;
    }

    template<typename TargetType, typename... OtherTypes>
    constexpr bool is_any_of_v = is_any_of<TargetType, OtherTypes...>::value;

    template<typename T>
    concept Pairable = requires(T value)
    {
        value.first;
        value.second;
    };

    template<typename T>
    concept DataAccessible = requires(T value)
    {
        value.data();
        value.size();
    };

    template<typename T>
    concept Iterable = requires(T value)
    {
        value.begin() != value.end();
    };

    template<typename T>
    concept CanBeStoredInIntegral =
        is_one_of(sizeof(T), sizeof(u8), sizeof(u16), sizeof(u32), sizeof(u64));

    template<typename T>
    concept RawAccessible =
        Iterable<T> && DataAccessible<T> && std::random_access_iterator<typename T::iterator>;

    template<typename T>
    concept ClassValueFastCopiable = std::is_trivially_copy_assignable_v<typename T::value_type>;

    template<typename CharT>
    concept CharacterLiteral =
        !std::is_array_v<CharT> && std::is_trivial_v<CharT> && std::is_standard_layout_v<CharT> &&
        cerb::is_any_of_v<CharT, char, unsigned char, char8_t, char16_t, char32_t>;
}// namespace cerb

#endif /* LIBCERBERUS_TYPE_HPP */
