#ifndef LIBCERBERUS_TYPE_HPP
#define LIBCERBERUS_TYPE_HPP

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
}// namespace cerb

#endif /* LIBCERBERUS_TYPE_HPP */
