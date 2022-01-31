#ifndef CERBERUS_FLAT_MAP_HPP
#define CERBERUS_FLAT_MAP_HPP

#include <cerberus/memory.hpp>
#include <cerberus/pair.hpp>

namespace cerb
{
    template<typename Key, typename Value, size_t Size>
    struct FlatMap
    {
        using KeyForFlatMap = AutoCopyType<Key>;
        using value_type = Pair<Key, Value, PairComparison::BY_FIRST_VALUE>;
        using storage_t = std::array<value_type, Size>;

        using iterator = typename storage_t::iterator;
        using const_iterator = typename storage_t::const_iterator;
        using reverse_iterator = typename storage_t::reverse_iterator;
        using const_reverse_iterator = typename storage_t::const_reverse_iterator;

        CERBLIB_DECL auto size() const -> size_t
        {
            return number_of_elems;
        }

        CERBLIB_DECL auto begin() -> iterator
        {
            return storage.begin();
        }

        CERBLIB_DECL auto begin() const -> const_iterator
        {
            return storage.begin();
        }

        CERBLIB_DECL auto end() -> iterator
        {
            return storage.begin() + number_of_elems;
        }

        CERBLIB_DECL auto end() const -> const_iterator
        {
            return storage.begin() + number_of_elems;
        }

        CERBLIB_DECL auto cbegin() const -> const_iterator
        {
            return storage.begin();
        }

        CERBLIB_DECL auto cend() const -> const_iterator
        {
            return storage.begin() + number_of_elems;
        }

        constexpr auto insert(value_type const &new_value) -> void
        {
            throwIfFull();
            storage[number_of_elems++] = new_value;
        }

        template<typename... Ts>
        constexpr auto emplace(KeyForFlatMap key, Ts &&...args) -> void
        {
            throwIfFull();
            storage[number_of_elems++] = { key, Value(args...) };
        }

        constexpr auto contains(KeyForFlatMap key) const -> bool
        {
            return findByKey(key) != end();
        }

        constexpr auto count(KeyForFlatMap key) const -> u32
        {
            return static_cast<u32>(contains(key));
        }

        constexpr auto operator[](KeyForFlatMap key) -> Value &
        {
            return at(key);
        }

        constexpr auto operator[](KeyForFlatMap key) const -> const Value &
        {
            return at(key);
        }

        constexpr auto at(KeyForFlatMap key) -> Value &
        {
            auto searched_elem = findByKey(key);

            if (searched_elem == end()) {
                this->emplace(key);
            }

            return searched_elem->second;
        }

        constexpr auto at(KeyForFlatMap key) const -> const Value &
        {
            auto searched_elem = findByKey(key);

            if (searched_elem == cend()) {
                throw std::out_of_range("Cerberus flat map does not contains given element!");
            }

            return searched_elem->second;
        }

        constexpr FlatMap() = default;
        constexpr FlatMap(std::initializer_list<value_type> const &storage_values)
        {
            for (const value_type &storage_value : storage_values) {
                insert(storage_value);
            }
        }

    private:
        CERBLIB_DECL auto findByKey(KeyForFlatMap key) -> iterator
        {
            return std::find(begin(), end(), key);
        }

        CERBLIB_DECL auto findByKey(KeyForFlatMap key) const -> const_iterator
        {
            return std::find(begin(), end(), key);
        }

        constexpr auto throwIfFull() const -> void
        {
            if (number_of_elems == storage.size()) {
                throw std::out_of_range("Cerberus flat map is full");
            }
        }

        storage_t storage{};
        size_t number_of_elems{ 0 };
    };
}// namespace cerb

#endif /* CERBERUS_FLAT_MAP_HPP */
