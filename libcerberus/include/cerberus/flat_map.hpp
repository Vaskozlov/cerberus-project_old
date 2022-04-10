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

        using iterator = GetIteratorType<storage_t>;
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
            throwWhenFull();
            storage[number_of_elems++] = new_value;
        }

        template<typename... Ts>
        constexpr auto emplace(KeyForFlatMap key, Ts &&...args) -> void
        {
            throwWhenFull();
            storage[number_of_elems++] = { key, Value(args...) };
        }

        CERBLIB_DECL auto contains(KeyForFlatMap key) const -> bool
        {
            return findByKey(key) != end();
        }

        CERBLIB_DECL auto count(KeyForFlatMap key) const -> u32
        {
            return static_cast<u32>(contains(key));
        }

        CERBLIB_DECL auto operator[](KeyForFlatMap key) -> Value &
        {
            return at(key);
        }

        CERBLIB_DECL auto operator[](KeyForFlatMap key) const -> Value const &
        {
            return at(key);
        }

        CERBLIB_DECL auto at(KeyForFlatMap key) -> Value &
        {
            auto searched_elem = findByKey(key);

            if (searched_elem == end()) {
                this->emplace(key);
            }

            return searched_elem->second;
        }

        CERBLIB_DECL auto at(KeyForFlatMap key) const -> Value const &
        {
            auto searched_elem = findByKey(key);

            if (searched_elem == cend()) {
                throw std::out_of_range("Cerberus flat map does not contains given element!");
            }

            return searched_elem->second;
        }

        FlatMap() = default;

        constexpr FlatMap(std::initializer_list<value_type> const &storage_values)
        {
            auto insert_value = [this](value_type const &value) { this->insert(value); };
            std::ranges::for_each(storage_values, insert_value);
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

        constexpr auto throwWhenFull() const -> void
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
