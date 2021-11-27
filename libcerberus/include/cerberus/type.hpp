#ifndef LIBCERBERUS_TYPE_HPP
#define LIBCERBERUS_TYPE_HPP

#include <memory>
#include <type_traits>

namespace cerb {
    template<typename TargetType, typename... OtherTypes>
    struct is_any_of
    {
        template<typename T, typename... Other>
        CERBLIB_DECL static auto check(bool result = false) -> bool
        {
            result = result || std::is_same_v<TargetType, T>;

            if constexpr (sizeof...(Other) != 0) {
                result = check<Other...>(result);
            }

            return result;
        }

        static constexpr auto value = check<OtherTypes...>();
    };

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
        value.begin();
        value.end();
    };

    template<typename T>
    concept RawAccessible = Iterable<T> && DataAccessible<T> &&
        std::random_access_iterator<typename T::iterator>;

    template<typename T>
    concept ClassValueFastCopiable = RawAccessible<T> &&
        std::is_trivially_copy_assignable_v<typename T::value_type> && requires(T value)
    {
        new int
            [static_cast<int64_t>(sizeof(u64)) -
             static_cast<int64_t>(sizeof(typename T::value_type))];
    };

    template<typename T>
    concept FastCopiable = std::is_trivially_copy_assignable_v<T> && requires(T value)
    {
        new int[static_cast<int64_t>(sizeof(u64)) - static_cast<int64_t>(sizeof(T))];
    };

    template<typename CharT>
    concept CharType =
        !std::is_array_v<CharT> && std::is_trivial_v<CharT> &&
        std::is_standard_layout_v<CharT> &&
        cerb::is_any_of_v<CharT, char, unsigned char, char8_t, char16_t, char32_t>;
}// namespace cerb

#endif /* LIBCERBERUS_TYPE_HPP */
