#include "memory.hpp"
#include <cerberus/pointer_wrapper.hpp>
#include <vector>

namespace cerb::test {

    constexpr i32 CheckValueI32 = -134;
    constexpr u8 TestU8Value = 255U;
    constexpr u16 TestU16Value = 1024U;
    constexpr u32 TestU32Value = 1U << 31;
    constexpr u64 TestU64Value = static_cast<u64>(1U) << 51;
    constexpr ComplexValue TestComplexValue = { -1515, 4019441.0 };

    constexpr auto arrayStoresSameValues(const auto &array, auto expected_value) -> bool
    {
        for (size_t i = 0; i < array.size(); ++i) {
            if (array[i] != expected_value) {
                return false;
            }
        }

        return true;
    }

    template<size_t ArraySize, typename T>
    constexpr auto copyRandomData2ArrayAndTestMemcpyOnIt(T *random_data) -> void
    {
        std::array<T, ArraySize> src{};
        std::array<T, ArraySize> dest{};

        memcpy(src.data(), random_data, ArraySize);
        EXPECT_TRUE(areObjectsInClassEqual<RawPointerWrapper<T>>(
            { src.data(), ArraySize }, { random_data, ArraySize }));

        memcpy(dest, src);
        EXPECT_TRUE(areObjectsInClassEqual(src, dest));
    }

    auto generateRandomComplexData(usize size) -> std::unique_ptr<ComplexValue>
    {
        static std::random_device random_device;
        static std::mt19937 engine(random_device());

        static std::uniform_int_distribution<isize> isize_distribution(
            std::numeric_limits<isize>::min(), std::numeric_limits<isize>::max());

        auto random_complex_data = std::unique_ptr<ComplexValue>(
            static_cast<ComplexValue *>(::operator new(size * sizeof(ComplexValue))));

        for (i64 i = 0; i < size; ++i) {
            random_complex_data.get()[i] = { isize_distribution(engine),
                                             static_cast<double>(isize_distribution(engine)) };
        }

        return random_complex_data;
    }

    consteval auto constexprMemsetTest() -> int
    {
        std::array<u8, 512> data_8{};
        std::array<u16, 512> data_16{};
        std::array<u32, 512> data_32{};
        std::array<u64, 512> data_64{};
        std::array<ComplexValue, 512> data_complex{};

        cerb::memset<u8>(data_8.data(), TestU8Value, 512);
        cerb::memset<u16>(data_16.data(), TestU16Value, 512);
        cerb::memset<u32>(data_32.data(), TestU32Value, 512);
        cerb::memset<u64>(data_64.data(), TestU64Value, 512);
        cerb::memset<ComplexValue>(data_complex.data(), TestComplexValue, 512);

        std::array<i32, 512> array_32{};
        cerb::memset(array_32, CheckValueI32);

        EXPECT_TRUE(arrayStoresSameValues(data_8, TestU8Value));
        EXPECT_TRUE(arrayStoresSameValues(data_16, TestU16Value));
        EXPECT_TRUE(arrayStoresSameValues(data_32, TestU32Value));
        EXPECT_TRUE(arrayStoresSameValues(data_64, TestU64Value));
        EXPECT_TRUE(arrayStoresSameValues(array_32, CheckValueI32));
        EXPECT_TRUE(arrayStoresSameValues(data_complex, TestComplexValue));

        return 0;
    }

    auto memsetTest(u32) -> void
    {
        constexpr u32 buffer_size = 512;
        constexpr u32 complex_buffer_size = 128;
        constexpr int const_result = constexprMemsetTest();
        static const std::string long_string = "Hello, world! It's a long string!";

        std::array<i32, 512> array_32{};
        std::vector<std::string> vector_str;
        std::vector<ComplexValue> complex_vector;
        std::array<std::string, complex_buffer_size> array_str;

        auto data = std::unique_ptr<ComplexValue>(
            static_cast<ComplexValue *>(::operator new(buffer_size * sizeof(ComplexValue))));
        auto *void_data = static_cast<void *>(data.get());

        auto data_8 = RawPointerWrapper<u8>(static_cast<u8 *>(void_data), buffer_size);
        auto data_16 = RawPointerWrapper<u16>(static_cast<u16 *>(void_data), buffer_size);
        auto data_32 = RawPointerWrapper<u32>(static_cast<u32 *>(void_data), buffer_size);
        auto data_64 = RawPointerWrapper<u64>(static_cast<u64 *>(void_data), buffer_size);
        auto data_complex = RawPointerWrapper<ComplexValue>(data.get(), 512);

        EXPECT_FALSE(const_result);

        cerb::memset<u8>(data_8.get(), TestU8Value, buffer_size);
        EXPECT_TRUE(arrayStoresSameValues(data_8, TestU8Value));

        cerb::memset<u16>(data_16.get(), TestU16Value, buffer_size);
        EXPECT_TRUE(arrayStoresSameValues(data_16, TestU16Value));

        cerb::memset<u32>(data_32.get(), TestU32Value, buffer_size);
        EXPECT_TRUE(arrayStoresSameValues(data_32, TestU32Value));

        cerb::memset<u64>(data_64.get(), TestU64Value, buffer_size);
        EXPECT_TRUE(arrayStoresSameValues(data_64, TestU64Value));

        cerb::memset<ComplexValue>(data_complex.get(), TestComplexValue, buffer_size);
        EXPECT_TRUE(arrayStoresSameValues(data_complex, TestComplexValue));

        cerb::memset(array_32, CheckValueI32);
        EXPECT_TRUE(arrayStoresSameValues(array_32, CheckValueI32));

        cerb::memset(array_str, long_string);
        EXPECT_TRUE(arrayStoresSameValues(array_str, long_string));

        vector_str.resize(complex_buffer_size);
        cerb::memset(vector_str, long_string);
        EXPECT_TRUE(arrayStoresSameValues(vector_str, long_string));

        complex_vector.resize(complex_buffer_size);
        cerb::memset(complex_vector, TestComplexValue);
        EXPECT_TRUE(arrayStoresSameValues(complex_vector, TestComplexValue));
    }

    auto memcpyTest() -> void
    {
        constexpr u32 buffer_size = 512;
        constexpr u32 complex_buffer_size = 128;
        static const std::string long_string = "Hello, world! It's a long string!";

        auto random_data_8 = createRandomArrayOfInts<u8>(buffer_size);
        auto random_data_16 = createRandomArrayOfInts<u16>(buffer_size);
        auto random_data_32 = createRandomArrayOfInts<u32>(buffer_size);
        auto random_data_usize = createRandomArrayOfInts<usize>(buffer_size);
        auto random_complex_data = generateRandomComplexData(complex_buffer_size);

        copyRandomData2ArrayAndTestMemcpyOnIt<buffer_size>(random_data_8.get());
        copyRandomData2ArrayAndTestMemcpyOnIt<buffer_size>(random_data_16.get());
        copyRandomData2ArrayAndTestMemcpyOnIt<buffer_size>(random_data_32.get());
        copyRandomData2ArrayAndTestMemcpyOnIt<buffer_size>(random_data_usize.get());
        copyRandomData2ArrayAndTestMemcpyOnIt<complex_buffer_size>(random_complex_data.get());
    }

    auto memoryTest(u32 argc) -> int
    {
        memsetTest(argc);
        memcpyTest();
        stringTest(argc);
        return 0;
    }
}// namespace cerb::test
