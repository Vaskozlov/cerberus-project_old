#ifndef LIBCERBERUS_TYPE_HPP
#define LIBCERBERUS_TYPE_HPP

#include <memory>
#include <type_traits>

namespace cerb {
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
        std::make_unique<int>(new int
                                  [static_cast<int64_t>(sizeof(u64)) -
                                   static_cast<int64_t>(sizeof(typename T::value_type))]);
    };

    template<typename T>
    concept FastCopiable = std::is_trivially_copy_assignable_v<T> && requires(T value)
    {
        std::make_unique<int>(
            new int[static_cast<int64_t>(sizeof(u64)) - static_cast<int64_t>(sizeof(T))]);
    };
}// namespace cerb

#endif /* LIBCERBERUS_TYPE_HPP */
