#include <cerberus/debug/debug.hpp>
#include <cerberus/debug/random_array.hpp>
#include <cerberus/pointer_wrapper.hpp>
#include <cerberus/range.hpp>

namespace cerb::debug
{
    auto testPointerWrapper() -> int
    {
        constexpr size_t test_array_size = 100;

        auto pointer = createRandomArrayOfInts<int>(test_array_size);
        auto pointer_wrapper = RawPointerWrapper<int>(pointer.data(), test_array_size);

        EXPECT_TRUE(pointer_wrapper.size() == test_array_size);

        EXPECT_TRUE([&]() {
            for (size_t i : Range(test_array_size)) {
                if (pointer[i] != pointer_wrapper[i]) {
                    return false;
                }
            }
            return true;
        }());

        EXPECT_TRUE([&]() {
            auto pointer_copy = pointer.data();

            for (int elem : pointer_wrapper) {
                if (elem != *pointer_copy) {
                    return false;
                }

                ++pointer_copy;
            }

            return true;
        }());

        return 0;
    }
}// namespace cerb::debug