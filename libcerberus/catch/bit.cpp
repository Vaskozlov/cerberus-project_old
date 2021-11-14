#include "bit.hpp"

namespace cerb::test {
    auto byte_mask_test() -> int
    {
        {
            ByteMask<i64> mask{ 0 };
            mask.mask_8[0] = 255;
            mask.mask_8[2] = 255;

            test([&mask]() { return mask() != 0xFF00FF; }, CERBLIB_LOCATION);

            mask.getAsInt() += 0xFF00;

            test([&mask]() { return mask() != 0xFFFFFF; }, CERBLIB_LOCATION);
        }
        return 0;
    }
}// namespace cerb::test
