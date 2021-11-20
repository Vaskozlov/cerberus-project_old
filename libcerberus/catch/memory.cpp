#include "memory.hpp"

namespace cerb::test {
    auto memory_test(u32 ) -> int
    {
        std::unique_ptr<u8> random_data_8 = random_array<u8>(1024);


        return 0;
    }
}// namespace cerb::test