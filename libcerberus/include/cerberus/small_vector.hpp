#ifndef CERBERUS_SMALL_VECTOR_HPP
#define CERBERUS_SMALL_VECTOR_HPP

#include <boost/container/small_vector.hpp>

namespace cerb
{
    template<typename T, size_t N = 8>
    using SmallVector = boost::container::small_vector<T, N>;
}

#endif /* CERBERUS_SMALL_VECTOR_HPP */
