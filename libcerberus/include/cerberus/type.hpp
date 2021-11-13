#ifndef LIBCERBERUS_TYPE_HPP
#define LIBCERBERUS_TYPE_HPP

namespace cerb {
    template<typename T>
    concept Pairable = requires(T value)
    {
        value.first;
        value.second;
    };
}// namespace cerb

#endif /* LIBCERBERUS_TYPE_HPP */
