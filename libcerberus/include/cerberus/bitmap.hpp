#ifndef CERBERUS_BITMAP_HPP
#define CERBERUS_BITMAP_HPP

#include <array>
#include <cerberus/bit.hpp>
#include <cerberus/bit_manipulation.hpp>
#include <cerberus/memory.hpp>
#include <memory>

namespace cerb
{
    template<size_t AxisN, size_t BitN>
    struct ConstBitmap
    {
        static_assert(AxisN != 0);

        constexpr static size_t length_of_axis =
            BitN / bitsizeof(size_t) + static_cast<size_t>(BitN % bitsizeof(size_t) != 0);
        constexpr static size_t npos = std::numeric_limits<size_t>::max();

        using axis_storage_t = std::array<size_t, length_of_axis>;
        using storage_t = std::array<axis_storage_t, AxisN>;

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
            fill(storage[Axis], static_cast<size_t>(0));
        }

        constexpr auto clear() -> void
        {
            auto clear_axis = [](axis_storage_t &axis_storage) {
                fill(axis_storage, static_cast<size_t>(0));
            };

            std::ranges::for_each(storage, clear_axis);
        }

        template<u16 BitValue, size_t Axis>
        constexpr auto set(size_t index) -> decltype(auto)
        {
            static_assert(Axis < AxisN);
            return bit::set<BitValue, size_t>(storage[Axis].begin(), index);
        }

        template<size_t Axis>
        constexpr auto at(size_t index) const -> decltype(auto)
        {
            static_assert(Axis < AxisN);
            return bit::at<size_t>(storage[Axis].begin(), index);
        }

        template<size_t Axis>
        constexpr auto reverseBits() -> void
        {
            static_assert(Axis < AxisN);

            auto reverse_value = [](size_t &value) { value = ~value; };
            std::ranges::for_each(storage[Axis], reverse_value);
        }

        template<bit::ValueOfBit... BitValues>
        CERBLIB_DECL auto find() const -> size_t
        {
            static_assert(AxisN == sizeof...(BitValues));

            size_t index = 0;

            for (; index < storage.size(); ++index) {
                auto suitable_bits = getSuitableBits<BitValues...>(index);

                if (suitable_bits != 0) {
                    return calculateBitPosition(index, suitable_bits);
                }
            }

            return npos;
        }

        constexpr ConstBitmap() = default;

    private:
        template<bit::ValueOfBit... BitValues>
        CERBLIB_DECL auto getSuitableBits(size_t index) const -> size_t
        {
            return bit::applyMaskOnArray<size_t, typename storage_t::const_iterator, BitValues...>(
                index, storage.begin());
        }

        CERBLIB_DECL auto calculateBitPosition(size_t index, size_t suitable_bits) const -> size_t
        {
            auto index_of_bit_in_number = bit::scanForward<1, size_t>(suitable_bits);
            auto bit_index = index * bitsizeof(size_t) + index_of_bit_in_number;
            return bit_index < BitN ? bit_index : npos;
        }

        storage_t storage{};
    };

}// namespace cerb

#endif /* CERBERUS_BITMAP_HPP */
