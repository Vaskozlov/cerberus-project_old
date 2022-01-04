#ifndef CERBERUS_BITMAP_HPP
#define CERBERUS_BITMAP_HPP

#include <array>
#include <cerberus/bit_manipulation.hpp>
#include <cerberus/bits.hpp>
#include <cerberus/memory.hpp>
#include <memory>

namespace cerb {

    template<size_t AxisN, size_t BitN>
    struct ConstBitmap
    {
        static_assert(AxisN != 0);

        using value_type = usize;
        using const_value_type = value_type;
        using pointer = value_type *;
        using const_pointer = const value_type *;

        constexpr static size_t length_of_axis =
            BitN / bitsizeof(value_type) + static_cast<size_t>(BitN % bitsizeof(value_type) != 0);
        constexpr static usize npos = std::numeric_limits<usize>::max();

        using axis_storage_t = std::array<value_type, length_of_axis>;
        using storage_t = std::array<axis_storage_t, AxisN>;

    private:
        storage_t storage{};

    public:
        CERBLIB_DECL auto size() const -> size_t
        {
            return BitN;
        }

        CERBLIB_DECL auto data() const -> storage_t
        {
            return storage;
        }

        CERBLIB_DECL auto numberOfAxis() const -> size_t
        {
            return AxisN;
        }

        template<size_t Axis>
        constexpr auto clear() -> void
        {
            static_assert(Axis <= AxisN);
            cerb::memset(storage[Axis], static_cast<value_type>(0));
        }

        constexpr auto clear() -> void
        {
            std::ranges::for_each(storage, [](axis_storage_t &axis_storage) {
                cerb::memset(axis_storage, static_cast<value_type>(0));
            });
        }

        template<u16 BitValue, usize Axis>
        constexpr auto set(size_t index) -> decltype(auto)
        {
            static_assert(Axis < AxisN);

            return bit::set<BitValue, usize>(storage[Axis].begin(), index);
        }

        template<size_t Axis>
        constexpr auto at(size_t index) const -> decltype(auto)
        {
            static_assert(Axis < AxisN);

            return bit::at<usize>(storage[Axis].begin(), index);
        }

        template<bit::ValueOfBit... BitValues>
        constexpr auto find() -> usize
        {
            static_assert(AxisN == sizeof...(BitValues));

            size_t index = 0;

            for (; index < storage.size(); ++index) {
                auto suitable_bits =
                    bit::applyMaskOnArray<usize, GetIteratorType<storage_t>, BitValues...>(
                        index, storage.begin());

                if (suitable_bits != 0) {
                    auto bit_index =
                        index * bitsizeof(usize) + bitScanForward<1, usize>(suitable_bits);
                    return bit_index < BitN ? bit_index : npos;
                }
            }

            return npos;
        }

        constexpr ConstBitmap() = default;
    };

}// namespace cerb

#endif /* CERBERUS_BITMAP_HPP */
