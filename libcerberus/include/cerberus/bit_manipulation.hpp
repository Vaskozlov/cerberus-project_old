#ifndef CERBERUS_BIT_MANIPULATION_HPP
#define CERBERUS_BIT_MANIPULATION_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/type.hpp>

namespace cerb::bit {
    enum struct ValueOfBit
    {
        ONE,
        ZERO,
        ANY
    };

    template<ValueOfBit... BitValues, std::unsigned_integral ValueType, typename T>
    CERBLIB_DECL auto applyMaskOnArray(size_t index_of_number, T array_iterator)
        -> ValueType
    {
        ValueType result = std::numeric_limits<ValueType>::max();

        forEach(
            [&array_iterator, &index_of_number](ValueOfBit bit_value){
                switch (bit_value) {
                case ValueOfBit::ONE:
                    result &= (*array_iterator)[index_of_number];
                    break;

                case ValueOfBit::ZERO:
                    result &= ~(*array_iterator)[index_of_number];

                default:
                    break;
                }
                ++array_iterator;
            }, BitValues...
        );

        return result;
    }

    template<std::integral T>
    constexpr auto at(const T *numbers, size_t index) -> bool
    {
        auto array_index = index / bitsizeof(T);
        auto bit_index   = index % bitsizeof(T);
        auto target_bit  = static_cast<T>(1) << bit_index;

        return (numbers[array_index] & target_bit) == target_bit;
    }

    template<std::integral T>
    constexpr auto at(const T &number, size_t index) -> decltype(auto)
    {
        auto bit_index  = index % bitsizeof(T);
        auto target_bit = static_cast<T>(1) << bit_index;

        return (number & target_bit) == target_bit;
    }

    template<Iterable T>
    constexpr auto at(const T &container, size_t index) -> decltype(auto)
    {
        static_assert(std::integral<GetValueType<T>>);

        auto array_index = index / bitsizeof(T);
        return at<GetValueType<T>>(*(std::begin(container) + array_index), index);
    }

    /**
     * Sets bit in the array of integers.
     * @tparam BitValue value of bit to set.
     * @tparam T type of numbers
     * @param numbers location where we need to set bit.
     * @param index index of the bit to set.
     * @return
     */
    template<u16 BitValue, std::integral T>
    constexpr auto set(T *numbers, size_t index) -> void
    {
        static_assert(BitValue == 0 || BitValue == 1, "Bit of value can be represented as 0 or 1");

        auto array_index = index / bitsizeof(T);
        auto bit_index   = index % bitsizeof(T);

        if constexpr (BitValue == 0) {
            numbers[array_index] &= ~(static_cast<T>(1) << bit_index);
        } else {
            numbers[array_index] |= static_cast<T>(1) << bit_index;
        }
    }

    /**
     * Sets bit in the number.
     * @tparam BitValue  value of bit to set.
     * @tparam T type of number
     * @param number number where we need to set bit.
     * @param index index of the bit to set.
     * @return
     */
    template<u16 BitValue, std::integral T>
    constexpr auto set(T &number, size_t index) -> decltype(auto)
    {
        auto bit_index = index % bitsizeof(T);

        if constexpr (BitValue == 0) {
            number &= ~(static_cast<T>(1) << bit_index);
        } else {
            number |= static_cast<T>(1) << bit_index;
        }
    }

    /**
     * @brief Sets bit in the array of integers.
     * @tparam BitValue value of bit to set.
     * @tparam T type of number
     * @param array array where integers are stored.
     * @param index index of the bit to set.
     * @return
     */
    template<u16 BitValue, Iterable T>
    constexpr auto set(T &container, size_t index) -> decltype(auto)
    {
        constexpr auto SizeOfType = bitsizeof(GetValueType<T>);

        auto array_index = index / SizeOfType;
        return set<BitValue>(*(std::begin(container) + array_index), index);
    }
}// namespace cerb::bit

#endif /* CERBERUS_BIT_MANIPULATION_HPP */
