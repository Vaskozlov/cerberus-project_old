#include "memory.hpp"
#include <vector>

namespace cerb::test {

    constexpr u8 testU8Value                = 255U;
    constexpr u16 testU16Value              = 1024U;
    constexpr u32 testU32Value              = 1U << 31;
    constexpr u64 testU64Value              = static_cast<u64>(1U) << 51;
    constexpr ComplexValue testComplexValue = { -1515, 4019441.0 };

    constexpr i32 CheckValueI32 = -134;

    consteval auto constexpr_memset_test() -> int
    {
        std::array<u8, 512> data_8{};
        std::array<u16, 512> data_16{};
        std::array<u32, 512> data_32{};
        std::array<u64, 512> data_64{};
        std::array<ComplexValue, 512> data_complex{};

        cerb::memset<u8>(data_8.data(), testU8Value, 512);
        cerb::memset<u16>(data_16.data(), testU16Value, 512);
        cerb::memset<u32>(data_32.data(), testU32Value, 512);
        cerb::memset<u64>(data_64.data(), testU64Value, 512);
        cerb::memset<ComplexValue>(data_complex.data(), testComplexValue, 512);

        std::array<i32, 512> array_32{};

        cerb::memset(array_32, CheckValueI32);

        auto &&array_check_function = [](const auto &array, auto expected) {
            for (size_t i = 0; i < array.size(); ++i) {
                if (array[i] != expected) {
                    return 0;
                }
            }
            return 1;
        };

        expect_true(array_check_function, CERBLIB_LOCATION, data_8, testU8Value);
        expect_true(array_check_function, CERBLIB_LOCATION, data_16, testU16Value);
        expect_true(array_check_function, CERBLIB_LOCATION, data_32, testU32Value);
        expect_true(array_check_function, CERBLIB_LOCATION, data_64, testU64Value);
        expect_true(array_check_function, CERBLIB_LOCATION, array_32, CheckValueI32);
        expect_true(
            array_check_function, CERBLIB_LOCATION, data_complex, testComplexValue);

        return 0;
    }

    auto memset_test(u32) -> void
    {
        constexpr u32 buffer_size            = 512;
        constexpr u32 complex_buffer_size    = 128;
        constexpr int const_result           = constexpr_memset_test();
        static const std::string long_string = "Hello, world! It's a long string!";

        std::array<i32, 512> array_32{};
        std::vector<std::string> vector_str;
        std::vector<ComplexValue> complex_vector;
        std::array<std::string, complex_buffer_size> array_str;

        auto &&buffer_check_function = [](const auto pointer, auto expected) {
            for (size_t i = 0; i < buffer_size; ++i) {
                if (pointer[i] != expected) {
                    return 0;
                }
            }
            return 1;
        };

        auto &&array_check_function = [](const auto &array, auto expected) {
            for (size_t i = 0; i < array.size(); ++i) {
                if (array[i] != expected) {
                    return 0;
                }
            }
            return 1;
        };

        auto data = std::unique_ptr<ComplexValue>(static_cast<ComplexValue *>(
            ::operator new(buffer_size * sizeof(ComplexValue))));

        auto *data_8  = static_cast<u8 *>(static_cast<void *>((data.get())));
        auto *data_16 = static_cast<u16 *>(static_cast<void *>((data.get())));
        auto *data_32 = static_cast<u32 *>(static_cast<void *>((data.get())));
        auto *data_64 = static_cast<u64 *>(static_cast<void *>((data.get())));
        ComplexValue *data_complex = data.get();

        expect_false([]() { return const_result; }, CERBLIB_LOCATION);

        cerb::memset<u8>(data_8, testU8Value, buffer_size);
        expect_true(buffer_check_function, CERBLIB_LOCATION, data_8, testU8Value);

        cerb::memset<u16>(data_16, testU16Value, buffer_size);
        expect_true(buffer_check_function, CERBLIB_LOCATION, data_16, testU16Value);

        cerb::memset<u32>(data_32, testU32Value, buffer_size);
        expect_true(buffer_check_function, CERBLIB_LOCATION, data_32, testU32Value);

        cerb::memset<u64>(data_64, testU64Value, buffer_size);
        expect_true(buffer_check_function, CERBLIB_LOCATION, data_64, testU64Value);

        cerb::memset<ComplexValue>(data_complex, testComplexValue, buffer_size);
        expect_true(
            buffer_check_function, CERBLIB_LOCATION, data_complex, testComplexValue);

        std::cout << cerb::ClassValueFastCopiable<i32> << std::endl;
        cerb::memset(array_32, CheckValueI32);
        array_check_function(array_32, CheckValueI32);

        cerb::memset(array_str, long_string);
        array_check_function(array_str, long_string);

        vector_str.resize(complex_buffer_size);
        cerb::memset(vector_str, long_string);
        array_check_function(vector_str, long_string);

        complex_vector.resize(complex_buffer_size);
        cerb::memset(complex_vector, testComplexValue);
        array_check_function(complex_vector, testComplexValue);
    }

    auto memcpy_test() -> void
    {
        constexpr u32 buffer_size = 512;
        // constexpr u32 complex_buffer_size    = 128;
        static const std::string long_string = "Hello, world! It's a long string!";

        auto &&buffer_check_function = [](const auto pointer, const auto expected) {
            for (size_t i = 0; i < buffer_size; ++i) {
                if (pointer[i] != expected[i]) {
                    return 0;
                }
            }
            return 1;
        };

        /*
        auto &&array_check_function = [](const auto &array, const auto &expected) {
            for (size_t i = 0; i < array.size(); ++i) {
                if (array[i] != expected[i]) {
                    return 0;
                }
            }
            return 1;
        };*/

        auto data = std::unique_ptr<u64>(
            static_cast<u64 *>(::operator new(buffer_size * sizeof(u64))));
        std::unique_ptr<u8> random_data_8   = random_array<u8>(buffer_size);
        std::unique_ptr<u16> random_data_16 = random_array<u16>(buffer_size);
        std::unique_ptr<u32> random_data_32 = random_array<u32>(buffer_size);
        std::unique_ptr<u64> random_data_64 = random_array<u64>(buffer_size);

        auto *data_8  = static_cast<u8 *>(static_cast<void *>((data.get())));
        auto *data_16 = static_cast<u16 *>(static_cast<void *>((data.get())));
        auto *data_32 = static_cast<u32 *>(static_cast<void *>((data.get())));
        auto *data_64 = static_cast<u64 *>(static_cast<void *>((data.get())));

        cerb::memcpy(data_8, random_data_8.get(), buffer_size);
        expect_true(buffer_check_function, CERBLIB_LOCATION, data_8, random_data_8.get());

        cerb::memcpy(data_16, random_data_16.get(), buffer_size);
        expect_true(
            buffer_check_function, CERBLIB_LOCATION, data_16, random_data_16.get());

        cerb::memcpy(data_32, random_data_32.get(), buffer_size);
        expect_true(
            buffer_check_function, CERBLIB_LOCATION, data_32, random_data_32.get());

        cerb::memcpy(data_64, random_data_64.get(), buffer_size);
        expect_true(
            buffer_check_function, CERBLIB_LOCATION, data_64, random_data_64.get());
    }

    auto memory_test(u32 argc) -> int
    {
        memset_test(argc);
        memcpy_test();
        return 0;
    }
}// namespace cerb::test
