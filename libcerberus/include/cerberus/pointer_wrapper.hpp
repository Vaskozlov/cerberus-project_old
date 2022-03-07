#ifndef CERBERUS_POINTER_WRAPPER_HPP
#define CERBERUS_POINTER_WRAPPER_HPP

#include <cerberus/memory.hpp>
#include <cerberus/type.hpp>

namespace cerb
{
    template<typename T>
    struct CERBLIB_TRIVIAL RawPointerWrapper
    {
        using element_type = T;
        using value_type = T;
        using iterator = T *;
        using const_iterator = T const *;
        using reverse_iterator = std::reverse_iterator<T *>;
        using const_reverse_iterator = std::reverse_iterator<T const *>;

        CERBLIB_DECL auto operator*() -> T &
        {
            return *pointer;
        }

        CERBLIB_DECL auto operator*() const -> T &
        {
            return *pointer;
        }

        CERBLIB_DECL auto operator->() -> T *
        {
            return pointer;
        }

        CERBLIB_DECL auto operator->() const -> T *
        {
            return pointer;
        }

        CERBLIB_DECL auto size() const -> size_t
        {
            return length;
        }

        CERBLIB_DECL auto get() const -> T *
        {
            return pointer;
        }

        template<typename U>
        CERBLIB_DECL explicit operator U *() const
        {
            return static_cast<U *>(pointer);
        }

        CERBLIB_DECL auto operator==(RawPointerWrapper const &other) -> bool
        {
            return equal(*this, other);
        }

        template<Iterable U>
        CERBLIB_DECL auto operator<=>(U const &other) -> decltype(auto)
        {
            for (size_t i = 0; i < min<size_t>(size(), other.size()); ++i) {
                if (*(pointer + i) != other[i]) {
                    return *(pointer + i) <=> other[i];
                }
            }

            return size() <=> other.size();
        }

        CERBLIB_DECL auto begin() -> iterator
        {
            return pointer;
        }

        CERBLIB_DECL auto end() -> iterator
        {
            return pointer + length;
        }

        CERBLIB_DECL auto begin() const -> iterator
        {
            return pointer;
        }

        CERBLIB_DECL auto end() const -> iterator
        {
            return pointer + length;
        }

        CERBLIB_DECL auto cbegin() const -> const_iterator
        {
            return pointer;
        }

        CERBLIB_DECL auto cend() const -> const_iterator
        {
            return pointer + length;
        }

        CERBLIB_DECL auto rbegin() -> reverse_iterator
        {
            return reverse_iterator(pointer + length);
        }

        CERBLIB_DECL auto rend() -> reverse_iterator
        {
            return reverse_iterator(pointer);
        }

        CERBLIB_DECL auto rbegin() const -> reverse_iterator
        {
            return reverse_iterator(pointer + length);
        }

        CERBLIB_DECL auto rend() const -> reverse_iterator
        {
            return reverse_iterator(pointer);
        }

        CERBLIB_DECL auto crbegin() const -> const_reverse_iterator
        {
            return reverse_iterator(pointer + length);
        }

        CERBLIB_DECL auto crend() const -> const_reverse_iterator
        {
            return reverse_iterator(pointer);
        }

        CERBLIB_DECL auto operator[](size_t index) -> T &
        {
            return pointer[index];
        }

        CERBLIB_DECL auto operator[](size_t index) const -> T &
        {
            return pointer[index];
        }

        constexpr RawPointerWrapper() = default;
        constexpr RawPointerWrapper(T *ptr, size_t len) : pointer(ptr), length(len)
        {}

    private:
        T *pointer{};
        size_t length{};
    };

    template<typename T>
    CERBLIB_DECL auto wrapPointer(T *ptr, size_t len) -> RawPointerWrapper<T>
    {
        return { ptr, len };
    }

    template<PointerLike T>
    CERBLIB_DECL auto wrapPointer(T &ptr, size_t len) -> RawPointerWrapper<typename T::element_type>
    {
        return { ptr.get(), len };
    }
}// namespace cerb

#endif /* CERBERUS_POINTER_WRAPPER_HPP */
