#ifndef CERBERUS_POINTER_WRAPPER_HPP
#define CERBERUS_POINTER_WRAPPER_HPP

#include <cerberus/memory.hpp>
#include <cerberus/type.hpp>

namespace cerb
{
    template<typename T>
    struct CERBLIB_TRIVIAL RawPointerWrapper
    {
        CERBLIB_DECL auto operator*() -> decltype(auto)
        {
            return *pointer;
        }

        CERBLIB_DECL auto operator*() const -> decltype(auto)
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

        using reverse_iterator = std::reverse_iterator<T *>;

        CERBLIB_DECL auto begin() const -> T *
        {
            return pointer;
        }

        CERBLIB_DECL auto end() const -> T *
        {
            return pointer + length;
        }

        CERBLIB_DECL auto cbegin() const -> T *
        {
            return pointer;
        }

        CERBLIB_DECL auto cend() const -> T *
        {
            return pointer + length;
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

        CERBLIB_DECL auto operator[](size_t index) -> T &
        {
            return pointer[index];
        }

        CERBLIB_DECL auto operator[](size_t index) const -> T &
        {
            return pointer[index];
        }

        constexpr auto set(T ptr, size_t len) -> void
        {
            pointer = ptr;
            length = len;
        }

        constexpr RawPointerWrapper() = default;
        constexpr RawPointerWrapper(T *ptr, size_t len) : pointer(ptr), length(len)
        {}

    private:
        T *pointer{};
        size_t length{};
    };
}// namespace cerb

#endif /* CERBERUS_POINTER_WRAPPER_HPP */
