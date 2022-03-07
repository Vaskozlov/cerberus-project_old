#include "fill.hpp"
#include <cerberus/debug/debug.hpp>
#include <cerberus/memory.hpp>
#include <string>

namespace cerb::debug
{
    template<typename T, size_t N = 512>
    CERBLIB_DECL auto testFillOnArray(T const &value) -> bool
    {
        auto same_to_value = [&value](T const &elem) { return safeEqual(elem, value); };

        std::array<T, N> data{};
        fill(data, value);

        return std::ranges::all_of(data, same_to_value);
    }

    template<typename T, size_t N = 512>
    CERBLIB_DECL auto testFillOnPointer(T const &value) -> bool
    {
        auto same_to_value = [&value](T const &elem) { return safeEqual(elem, value); };

        std::array<T, N> data{};
        fill(data.data(), value, N);

        return std::ranges::all_of(data, same_to_value);
    }

    CERBLIB_DECL auto testFillOnArrayOfInts() -> bool
    {
        EXPECT_TRUE(testFillOnArray<u8>(0xAC));
        EXPECT_TRUE(testFillOnArray<u16>(0xABCD));
        EXPECT_TRUE(testFillOnArray<u32>(0x1234'ABCD));

        if constexpr (sizeof(u64) != sizeof(u32)) {
            EXPECT_TRUE(testFillOnArray<u64>(0x1234'5678'9ABC'DEF0));
        }

        return true;
    }

    CERBLIB_DECL auto testFillOnArrayOfFloats() -> bool
    {
        EXPECT_TRUE(testFillOnArray<float>(45151.41551f));
        EXPECT_TRUE(testFillOnArray<double>(1451985155195.51851051521));
        return true;
    }


    CERBLIB_DECL auto testFillOnPointerOfInts() -> bool
    {
        EXPECT_TRUE(testFillOnPointer<u8>(0xAC));
        EXPECT_TRUE(testFillOnPointer<u16>(0xABCD));
        EXPECT_TRUE(testFillOnPointer<u32>(0x1234'ABCD));

        if constexpr (sizeof(u64) != sizeof(u32)) {
            EXPECT_TRUE(testFillOnPointer<u64>(0x1234'5678'9ABC'DEF0));
        }

        return true;
    }

    CERBLIB_DECL auto testFillOnPointerOfFloats() -> bool
    {
        EXPECT_TRUE(testFillOnPointer<float>(45151.41551f));
        EXPECT_TRUE(testFillOnPointer<double>(1451985155195.51851051521));
        return true;
    }

    auto testFillOnArrayOfStrings() -> bool
    {
        std::string const long_string = "Hello, world! It's a long string!";
        EXPECT_TRUE(testFillOnArray<std::string>(long_string));
        return true;
    }

    auto testFillOnPointerOfStrings() -> bool
    {
        std::string const long_string = "Hello, world! It's a long string!";
        EXPECT_TRUE(testFillOnPointer<std::string>(long_string));
        return true;
    }

    auto testFill() -> int
    {
        CR_EXPECT_TRUE(testFillOnArrayOfInts());
        CR_EXPECT_TRUE(testFillOnArrayOfFloats());
        CR_EXPECT_TRUE(testFillOnPointerOfInts());
        CR_EXPECT_TRUE(testFillOnPointerOfFloats());

        EXPECT_TRUE(testFillOnArrayOfStrings());
        EXPECT_TRUE(testFillOnPointerOfStrings());

        return 0;
    }
}// namespace cerb::debug
