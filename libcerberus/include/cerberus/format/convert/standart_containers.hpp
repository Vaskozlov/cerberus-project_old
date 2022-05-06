#ifndef CERBERUS_STANDART_CONTAINERS_HPP
#define CERBERUS_STANDART_CONTAINERS_HPP

#include <cerberus/format/convert/iterable.hpp>

#define CCHAR(x) static_cast<CharT>(x)

// NOLINTNEXTLINE
namespace std
{
    template<typename Key, typename Compare, typename Alloc>
    struct set;

    template<typename T, typename Alloc>
    struct vector;

    template<typename T, typename Alloc>
    struct deque;
}// namespace std

namespace cerb::fmt
{
    template<CharacterLiteral CharT, typename T, typename Alloc>
    constexpr auto convert(std::vector<T, Alloc> const &iterable_obj) -> std::basic_string<CharT>
    {
        using namespace private_;

        return convertIterable<CharT, std::vector<T, Alloc>>('[', ']', iterable_obj);
    }

    template<CharacterLiteral CharT, typename T, typename Alloc>
    constexpr auto convert(std::deque<T, Alloc> const &iterable_obj) -> std::basic_string<CharT>
    {
        using namespace private_;

        return convertIterable<CharT, std::deque<T, Alloc>>('[', ']', iterable_obj);
    }

    template<CharacterLiteral CharT, typename Key, typename Compare, typename Alloc>
    constexpr auto convert(std::set<Key, Compare, Alloc> const &iterable_obj)
        -> std::basic_string<CharT>
    {
        using namespace private_;

        return convertIterable<CharT, std::set<Key, Compare, Alloc>>('{', '}', iterable_obj);
    }
}// namespace cerb::fmt

#undef CCHAR

#endif /* CERBERUS_STANDART_CONTAINERS_HPP */
