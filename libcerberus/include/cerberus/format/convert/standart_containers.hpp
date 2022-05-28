#ifndef CERBERUS_STANDART_CONTAINERS_HPP
#define CERBERUS_STANDART_CONTAINERS_HPP

#include <cerberus/format/convert/iterable.hpp>

#define CCHAR(x) static_cast<CharT>(x)

// NOLINTNEXTLINE
namespace std
{
    template<typename T, size_t N>
    struct array;

    template<typename Key, typename Compare, typename Alloc>
    struct set;

    template<typename T, typename Alloc>
    struct vector;

    template<typename T, typename Alloc>
    struct deque;

    template<typename T1, typename T2>
    struct pair;
}// namespace std

namespace cerb
{
    enum struct PairComparison : u16;

    template<typename T1, typename T2, PairComparison>
    class Pair;
}// namespace cerb

namespace cerb::fmt
{
    template<typename CharT, typename... Ts>
    CERBLIB_DECL auto format(BasicStringView<char> const &formatter, Ts &&...args)
        -> std::basic_string<CharT>;

    template<CharacterLiteral CharT, typename T1, typename T2>
    CERBLIB_DECL auto convert(std::pair<T1, T2> const &pair) -> std::basic_string<CharT>
    {
        using namespace private_;

        return format<CharT>("{{{}, {}}}", pair.first, pair.second);
    }

    template<CharacterLiteral CharT, typename T1, typename T2, PairComparison Rule>
    CERBLIB_DECL auto convert(Pair<T1, T2, Rule> const &pair) -> std::basic_string<CharT>
    {
        using namespace private_;

        return format<CharT>("{{{}, {}}}", pair.first, pair.second);
    }

    template<CharacterLiteral CharT, typename T, size_t N>
    CERBLIB_DECL auto convert(std::array<T, N> const &iterable_obj) -> std::basic_string<CharT>
    {
        using namespace private_;

        return convertIterable<CharT, std::array<T, N>>('[', ']', iterable_obj);
    }

    template<CharacterLiteral CharT, typename T, typename Alloc>
    CERBLIB_DECL auto convert(std::vector<T, Alloc> const &iterable_obj) -> std::basic_string<CharT>
    {
        using namespace private_;

        return convertIterable<CharT, std::vector<T, Alloc>>('[', ']', iterable_obj);
    }

    template<CharacterLiteral CharT, typename T, typename Alloc>
    CERBLIB_DECL auto convert(std::deque<T, Alloc> const &iterable_obj) -> std::basic_string<CharT>
    {
        using namespace private_;

        return convertIterable<CharT, std::deque<T, Alloc>>('[', ']', iterable_obj);
    }

    template<CharacterLiteral CharT, typename Key, typename Compare, typename Alloc>
    CERBLIB_DECL auto convert(std::set<Key, Compare, Alloc> const &iterable_obj)
        -> std::basic_string<CharT>
    {
        using namespace private_;

        return convertIterable<CharT, std::set<Key, Compare, Alloc>>('{', '}', iterable_obj);
    }
}// namespace cerb::fmt

#undef CCHAR

#endif /* CERBERUS_STANDART_CONTAINERS_HPP */
