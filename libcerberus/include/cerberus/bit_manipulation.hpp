#ifndef CERBERUS_BIT_MANIPULATION_HPP
#define CERBERUS_BIT_MANIPULATION_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/number.hpp>
#include <cerberus/type.hpp>

namespace cerb::bit
{
    enum struct ValueOfBit : u16
    {
        ONE,
        ZERO,
        ANY
    };

    template<std::integral ValueType, typename Iterator>
    constexpr auto at(Iterator iterator, size_t index) -> bool
    {
        auto array_index = index / bitsizeof(ValueType);
        auto bit_index = index % bitsizeof(ValueType);
        auto target_bit = static_cast<ValueType>(1) << bit_index;

        return (*(iterator + array_index) & target_bit) == target_bit;
    }

    template<std::integral T>
    constexpr auto at(T const &number, size_t index) -> bool
    {
        auto bit_index = index % bitsizeof(T);
        size_t target_bit = pow2<size_t>(bit_index);

        return (number & target_bit) == target_bit;
    }

    template<u16 BitValue, std::integral ValueType, typename Iterator>
    constexpr auto set(Iterator iterator, size_t index) -> void
    {
        static_assert(isOneOf(BitValue, 0, 1), "Bit of value can be represented as 0 or 1");

        auto array_index = index / bitsizeof(ValueType);
        auto bit_index = index % bitsizeof(ValueType);

        if constexpr (BitValue == 0) {
            *(iterator + array_index) &= ~(pow2<ValueType>(bit_index));
        } else {
            *(iterator + array_index) |= pow2<ValueType>(bit_index);
        }
    }

    template<u16 BitValue, std::integral T>
    constexpr auto set(T &number, size_t index) -> decltype(auto)
    {
        auto bit_index = index % bitsizeof(T);

        if constexpr (BitValue == 0) {
            number &= ~(pow2<T>(bit_index));
        } else {
            number |= pow2<T>(bit_index);
        }
    }

    template<std::unsigned_integral ValueType, typename T, ValueOfBit... BitValues>
    CERBLIB_DECL auto applyMaskOnArray(size_t index, T array_iterator) -> ValueType
    {
        ValueType result = std::numeric_limits<ValueType>::max();
        auto process_layer_of_mask = [&array_iterator, &index, &result](ValueOfBit bit_value) {
            switch (bit_value) {
            case ValueOfBit::ONE:
                result &= (*array_iterator)[index];
                break;

            case ValueOfBit::ZERO:
                result &= ~(*array_iterator)[index];

            default:
                break;
            }
            ++array_iterator;
        };

        forEach(process_layer_of_mask, BitValues...);
        return result;
    }
}// namespace cerb::bit

#endif /* CERBERUS_BIT_MANIPULATION_HPP */
