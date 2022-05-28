#include <cerberus/debug/debug.hpp>
#include <cerberus/debug/random_values.hpp>
#include <cerberus/memory.hpp>
#include <cerberus/pointer_wrapper.hpp>

namespace cerb::debug
{
    template<size_t N, Iterable T>
    CERBLIB_DECL auto testCopyFromIterableToArray(T const &source) -> bool
    {
        std::array<GetValueType<T>, N> dest{};
        copy(dest, source);
        return std::ranges::equal(source, dest);
    }

    template<size_t N, typename T>
    CERBLIB_DECL auto testCopyFromIterableToPointer(T const *source) -> bool
    {
        std::array<T, N> dest{};
        copy(dest.data(), source, N);
        return std::ranges::equal(wrapPointer(source, N), dest);
    }

    CERBLIB_DECL auto testCopyOnArray() -> bool
    {
        ASSERT_TRUE(testCopyFromIterableToArray<RandomBytes.size()>(RandomBytes));
        ASSERT_TRUE(testCopyFromIterableToArray<RandomWords.size()>(RandomWords));
        ASSERT_TRUE(testCopyFromIterableToArray<RandomDWords.size()>(RandomDWords));

#if CERBLIB_64BIT
        ASSERT_TRUE(testCopyFromIterableToArray<RandomQWords.size()>(RandomQWords));
#endif
        return true;
    }

    CERBLIB_DECL auto testCopyOnPointer() -> bool
    {
        ASSERT_TRUE(testCopyFromIterableToPointer<RandomBytes.size()>(RandomBytes.data()));
        ASSERT_TRUE(testCopyFromIterableToPointer<RandomWords.size()>(RandomWords.data()));
        ASSERT_TRUE(testCopyFromIterableToPointer<RandomDWords.size()>(RandomDWords.data()));

#if CERBLIB_64BIT
        ASSERT_TRUE(testCopyFromIterableToPointer<RandomQWords.size()>(RandomQWords.data()));
#endif
        return true;
    }

    auto testCopy() -> int
    {
        CERBERUS_TEST(testCopyOnArray());
        CERBERUS_TEST(testCopyOnPointer());
        return 0;
    }
}// namespace cerb::debug
