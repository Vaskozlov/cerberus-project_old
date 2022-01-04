#ifndef CERBERUS_POINTER_WRAPPER_HPP
#define CERBERUS_POINTER_WRAPPER_HPP

#include <cerberus/memory.hpp>
#include <cerberus/type.hpp>

namespace cerb {
    template<typename T, typename ValueType>
    struct PointerWrapper
    {
        using value_type = ValueType;

        CERBLIB_DECL auto operator*() -> decltype(auto)
        {
            return *pointer;
        }

        CERBLIB_DECL auto operator*() const -> decltype(auto)
        {
            return *pointer;
        }

        CERBLIB_DECL auto operator->() -> T
        {
            return pointer;
        }

        CERBLIB_DECL auto operator->() const -> T
        {
            return pointer;
        }

        CERBLIB_DECL auto size() const -> size_t
        {
            return length;
        }

        CERBLIB_DECL auto get() const -> T
        {
            return pointer;
        }

        CERBLIB_DECL auto operator==(const PointerWrapper &other) -> bool
        {
            return areObjectsInClassEqual(*this, other);
        }

        CERBLIB_DECL auto operator<=>(const PointerWrapper &other) -> decltype(auto)
        {
            for (size_t i = 0; i < min<size_t>(size(), other.size()); ++i) {
                if (*(pointer + i) != *(other.pointer + i)) {
                    return *(pointer + i) <=> *(other.pointer + i);
                }
            }

            return size() <=> other.size();
        }

        struct iterator
        {
            using iterator_category = std::contiguous_iterator_tag;

            CERBLIB_DECL auto operator*() -> decltype(auto)
            {
                return *pointer;
            }

            CERBLIB_DECL auto operator*() const -> decltype(auto)
            {
                return *pointer;
            }

            CERBLIB_DECL auto operator->() -> T
            {
                return pointer;
            }

            CERBLIB_DECL auto operator->() const -> T
            {
                return pointer;
            }

            CERBLIB_DECL auto operator+(const iterator &other) const -> iterator
            {
                return iterator(pointer + other.pointer);
            }

            CERBLIB_DECL auto operator-(const iterator &other) const -> iterator
            {
                return iterator(pointer - other.pointer);
            }

            constexpr auto operator++() -> iterator &
            {
                ++pointer;
                return *this;
            }

            constexpr auto operator++(int) -> iterator
            {
                iterator copy_of_iterator = *this;
                ++pointer;
                return copy_of_iterator;
            }

            constexpr auto operator--() -> iterator &
            {
                --pointer;
                return *this;
            }

            constexpr auto operator--(int) -> iterator
            {
                iterator copy_of_iterator = *this;
                --pointer;
                return copy_of_iterator;
            }

            CERBLIB_DECL auto operator<=>(const iterator &) const = default;

            constexpr iterator() = default;
            constexpr iterator(T ptr) : pointer(ptr)
            {}

        private:
            T pointer{};
        };

        using reverse_iterator = std::reverse_iterator<iterator>;

        CERBLIB_DECL auto begin() const -> iterator
        {
            return iterator(pointer);
        }

        CERBLIB_DECL auto end() const -> iterator
        {
            return iterator(pointer + length);
        }

        CERBLIB_DECL auto cbegin() const -> iterator
        {
            return iterator(pointer);
        }

        CERBLIB_DECL auto cend() const -> iterator
        {
            return iterator(pointer + length);
        }

        CERBLIB_DECL auto rbegin() const -> reverse_iterator
        {
            return reverse_iterator(pointer + length);
        }

        CERBLIB_DECL auto rend() const -> reverse_iterator
        {
            return reverse_iterator(pointer);
        }

        CERBLIB_DECL auto crbegin() const -> reverse_iterator
        {
            return reverse_iterator(pointer + length);
        }

        CERBLIB_DECL auto crend() const -> reverse_iterator
        {
            return reverse_iterator(pointer);
        }

        CERBLIB_DECL auto operator[](size_t index) -> decltype(auto)
        {
            return pointer[index];
        }

        CERBLIB_DECL auto operator[](size_t index) const -> decltype(auto)
        {
            return pointer[index];
        }

        constexpr auto set(T ptr, size_t len) -> void
        {
            pointer = ptr;
            length = len;
        }

        constexpr PointerWrapper() = default;
        constexpr PointerWrapper(T ptr, size_t len) : pointer(ptr), length(len)
        {}

    private:
        T pointer{};
        size_t length{};
    };

    template<typename T>
    using RawPointerWrapper = PointerWrapper<T *, T>;
}// namespace cerb

#endif /* CERBERUS_POINTER_WRAPPER_HPP */
