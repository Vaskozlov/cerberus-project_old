#include "pointer_wrapper.hpp"
#include <cerberus/debug/random_array.hpp>
#include <cerberus/pointer_wrapper.hpp>

namespace cerb::debug
{
    auto testPointerWrapper(u32) -> int
    {
        auto pointer = createRandomArrayOfInts<int>(100);
        auto pointer_wrapper = RawPointerWrapper<int>(pointer.get(), 100);

        EXPECT_TRUE(pointer_wrapper.size() == 100);

        EXPECT_TRUE([&]() {
            for (size_t i = 0; i < 100; ++i) {
                if (pointer.get()[i] != pointer_wrapper[i]) {
                    return false;
                }
            }
            return true;
        }());

        EXPECT_TRUE([&]() {
            auto pointer_copy = pointer.get();

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