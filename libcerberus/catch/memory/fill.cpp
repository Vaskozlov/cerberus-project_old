#include <cerberus/debug/debug.hpp>
#include <cerberus/memory.hpp>
#include <string>

namespace cerb::debug
{
    static constexpr size_t TestLength = 512;
    static constexpr float TestFloatValue = 45151.41551F;
    static constexpr double TestDoubleValue = 1451985155195.51851051521;

    template<typename T, size_t N = TestLength>
    CERBLIB_DECL auto testFillOnArray(T const &value) -> bool
    {
        auto same_to_value = [&value](T const &elem) { return safeEqual(elem, value); };

        std::array<T, N> data{};
        fill(data, value);

        return std::ranges::all_of(data, same_to_value);
    }

    template<typename T, size_t N = TestLength>
    CERBLIB_DECL auto testFillOnPointer(T const &value) -> bool
    {
        auto same_to_value = [&value](T const &elem) { return safeEqual(elem, value); };

        std::array<T, N> data{};
        fill(data.data(), value, N);

        return std::ranges::all_of(data, same_to_value);
    }

    CERBERUS_TEST_FUNC(testFillOnArrayOfInts)
    {
        ASSERT_TRUE(testFillOnArray<u8>(0xAC));
        ASSERT_TRUE(testFillOnArray<u16>(0xABCD));
        ASSERT_TRUE(testFillOnArray<u32>(0x1234'ABCD));

        if constexpr (sizeof(u64) != sizeof(u32)) {
            ASSERT_TRUE(testFillOnArray<u64>(0x1234'5678'9ABC'DEF0));
        }

        return true;
    }

    CERBERUS_TEST_FUNC(testFillOnArrayOfFloats)
    {
        ASSERT_TRUE(testFillOnArray<float>(TestFloatValue));
        ASSERT_TRUE(testFillOnArray<double>(TestDoubleValue));
        return true;
    }


    CERBERUS_TEST_FUNC(testFillOnPointerOfInts)
    {
        ASSERT_TRUE(testFillOnPointer<u8>(0xAC));
        ASSERT_TRUE(testFillOnPointer<u16>(0xABCD));
        ASSERT_TRUE(testFillOnPointer<u32>(0x1234'ABCD));

        if constexpr (sizeof(u64) != sizeof(u32)) {
            ASSERT_TRUE(testFillOnPointer<u64>(0x1234'5678'9ABC'DEF0));
        }

        return true;
    }

    CERBERUS_TEST_FUNC(testFillOnPointerOfFloats)
    {
        ASSERT_TRUE(testFillOnPointer<float>(TestFloatValue));
        ASSERT_TRUE(testFillOnPointer<double>(TestDoubleValue));
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFillOnArrayOfStrings)
    {
        std::string const long_string = "Hello, world! It's a long string!";
        ASSERT_TRUE(testFillOnArray<std::string>(long_string));
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFillOnPointerOfStrings)
    {
        std::string const long_string = "Hello, world! It's a long string!";
        ASSERT_TRUE(testFillOnPointer<std::string>(long_string));
        return true;
    }

    auto testFill() -> int
    {
        CERBERUS_TEST(testFillOnArrayOfInts());
        CERBERUS_TEST(testFillOnArrayOfFloats());
        CERBERUS_TEST(testFillOnPointerOfInts());
        CERBERUS_TEST(testFillOnPointerOfFloats());

        CERBERUS_TEST_STD_STRING(testFillOnArrayOfStrings());
        CERBERUS_TEST_STD_STRING(testFillOnPointerOfStrings());

        return 0;
    }
}// namespace cerb::debug
