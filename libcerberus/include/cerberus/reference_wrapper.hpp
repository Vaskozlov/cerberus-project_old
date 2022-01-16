#ifndef CERBERUS_REFERENCE_WRAPPER_HPP
#define CERBERUS_REFERENCE_WRAPPER_HPP

#include <cerberus/cerberus.hpp>

namespace cerb {
    template<typename T>
    struct CERBLIB_TRIVIAL ReferenceWrapper
    {
        CERBLIB_DECL auto get() -> T &
        {
            return reference;
        }

        CERBLIB_DECL auto get() const -> T &
        {
            return reference;
        }

        constexpr ReferenceWrapper() = default;
        constexpr explicit ReferenceWrapper(T &ref) : reference(ref)
        {}

    private:
        T &reference;
    };

    template<typename T>
    CERBLIB_DECL auto reference(T &ref) -> ReferenceWrapper<T>
    {
        return ReferenceWrapper(ref);
    }

    template<typename T>
    CERBLIB_DECL auto reference(ReferenceWrapper<T> ref) -> ReferenceWrapper<T>
    {
        return ref;
    }
}// namespace cerb

#endif /* CERBERUS_REFERENCE_WRAPPER_HPP */
