#ifndef CERBERUS_RANDOM_ARRAY_HPP
#define CERBERUS_RANDOM_ARRAY_HPP

#include <cerberus/range.hpp>
#include <memory>
#include <random>

namespace cerb::debug
{
    template<std::integral T>
    auto createRandomArrayOfInts(size_t size) -> std::unique_ptr<T[]>
    {
        using value_type = std::conditional_t<std::is_unsigned_v<T>, size_t, ssize_t>;

        static std::random_device random_device;
        static std::mt19937 engine(random_device());
        static std::uniform_int_distribution<value_type> distribution(
            std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

        std::unique_ptr<T[]> data = std::make_unique<T[]>(size);

        for (size_t i : Range(size)) {
            data.get()[i] = static_cast<T>(distribution(engine));
        }

        return data;
    }

    template<Pairable T>
    auto createRandomArrayOfPairs(size_t size) -> std::unique_ptr<T[]>
    {
        using pair_v1 = decltype(T{}.first);
        using pair_v2 = decltype(T{}.second);

        using pair_v1_for_generation =
            std::conditional_t<std::is_floating_point_v<pair_v1>, ssize_t, pair_v1>;
        using pair_v2_for_generation =
            std::conditional_t<std::is_floating_point_v<pair_v2>, ssize_t, pair_v2>;

        using value_type1 = std::conditional_t<std::unsigned_integral<pair_v1>, size_t, ssize_t>;
        using value_type2 = std::conditional_t<std::unsigned_integral<pair_v2>, size_t, ssize_t>;

        static std::random_device random_device;
        static std::mt19937 engine(random_device());

        static std::uniform_int_distribution<value_type1> first_distribution(
            std::numeric_limits<pair_v1_for_generation>::min(),
            std::numeric_limits<pair_v1_for_generation>::max());

        static std::uniform_int_distribution<value_type2> second_distribution(
            std::numeric_limits<pair_v2_for_generation>::min(),
            std::numeric_limits<pair_v2_for_generation>::max());
        
        std::unique_ptr<T[]> data = std::make_unique<T[]>(size);

        for (size_t i : Range(size)) {
            data.get()[i].first = static_cast<pair_v1>(first_distribution(engine));
            data.get()[i].second = static_cast<pair_v2>(second_distribution(engine));
        }

        return data;
    }
}// namespace cerb::debug

#endif /* CERBERUS_RANDOM_ARRAY_HPP */
