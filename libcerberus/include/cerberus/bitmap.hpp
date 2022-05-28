#ifndef CERBERUS_BITMAP_HPP
#define CERBERUS_BITMAP_HPP

#include <cerberus/bit.hpp>
#include <cerberus/bit_manipulation.hpp>
#include <cerberus/memory.hpp>
#include <vector>

namespace cerb
{
    class Bitmap
    {
    public:
        constexpr auto clear() -> void
        {
            fill(storage, 0);
        }

        template<u16 BitValue>
        constexpr auto set(size_t index) -> void
        {
            checkStorageCapacity(index);
            bit::set<BitValue, size_t>(storage.begin(), index);
        }

        template<u16 BitValue>
        constexpr auto multiSet(size_t from, size_t to) -> void
        {
            checkStorageCapacity(to);

            CERBLIB_UNROLL_N(4)
            for (; from != to; ++from) {
                bit::set<BitValue, size_t>(storage.begin(), from);
            }
        }

        CERBLIB_DECL auto at(size_t index) const -> bool
        {
            if (indexOutOfCapacity(index)) {
                return false;
            }

            return bit::at<size_t>(storage.data(), index);
        }

        CERBLIB_DECL auto empty() const -> bool
        {
            return storage.empty();
        }

        Bitmap() = default;

    private:
        CERBLIB_DECL auto indexOutOfCapacity(size_t index) const -> bool
        {
            auto storage_index = index / bitsizeof(size_t);
            return storage_index >= storage.size();
        }

        constexpr auto checkStorageCapacity(size_t index) -> void
        {
            constexpr size_t additional_block = 2;

            if (index >= storage.size() * bitsizeof(size_t)) {
                storage.resize(index / bitsizeof(size_t) + additional_block, 0);
            }
        }

        std::vector<size_t> storage{};
    };
}// namespace cerb

#endif /* CERBERUS_BITMAP_HPP */
