#include "bit.hpp"

namespace cerb::test {

    auto test_mask() -> void
    {
        ByteMask<i64> mask{ 0 };
        mask.mask_8[0] = 255;
        mask.mask_8[2] = 255;

        test([&mask]() { return mask() != 0xFF00FF; }, CERBLIB_LOCATION);

        mask.getAsInt() += 0xFF00;

        test([&mask]() { return mask() != 0xFFFFFF; }, CERBLIB_LOCATION);
    }

    auto test_min_max() -> void
    {
        test(
            []() {
                int v_min = min(10, -41, 14515, 0);
                return v_min != -41;
            },
            CERBLIB_LOCATION);

        test(
            []() {
                int v_max = max(10, -41, 14515, 0);
                return v_max != 14515;
            },
            CERBLIB_LOCATION);

        int counter = 0;

        test(
            [&counter]() {
                for_each(
                    [&counter](const auto &elem) {
                        test(
                            [&counter, &elem]() { return elem != counter * 10; },
                            CERBLIB_LOCATION);
                        ++counter;
                    },
                    0,
                    10,
                    20,
                    30,
                    40,
                    50);
                return 0;
            },
            CERBLIB_LOCATION);
    }

    auto test_pow2() -> void
    {
        test([]() { return pow2<u32>(0U) != 1U; }, CERBLIB_LOCATION);
        test([]() { return pow2<u32>(2U) != 4U; }, CERBLIB_LOCATION);
        test([]() { return pow2<u32>(10U) != 1024U; }, CERBLIB_LOCATION);
        test([]() { return pow2<u64>(54ULL) != (1ULL << 54ULL); }, CERBLIB_LOCATION);

        test([]() { return not_equal(pow2<float>(0U), 1.0f); }, CERBLIB_LOCATION);
        test([]() { return not_equal(pow2<float>(2U), 4.0f); }, CERBLIB_LOCATION);
        test([]() { return not_equal(pow2<float>(10U), 1024.0f); }, CERBLIB_LOCATION);
        test(
            []() {
                return not_equal(pow2<float>(54ULL), static_cast<float>((1ULL << 54ULL)));
            },
            CERBLIB_LOCATION);

        test([]() { return not_equal(pow2<double>(0U), 1.0); }, CERBLIB_LOCATION);
        test([]() { return not_equal(pow2<double>(2U), 4.0); }, CERBLIB_LOCATION);
        test([]() { return not_equal(pow2<double>(10U), 1024.0); }, CERBLIB_LOCATION);
        test(
            []() {
                return not_equal(
                    pow2<double>(54ULL), static_cast<double>((1ULL << 54ULL)));
            },
            CERBLIB_LOCATION);
    }

    auto test_abs() -> void
    {
        test([]() { return abs(0) != 0; }, CERBLIB_LOCATION);
        test([]() { return abs(10) != 10; }, CERBLIB_LOCATION);

        test([]() { return abs(-10) != 10; }, CERBLIB_LOCATION);
        test([]() { return abs<u32>(-10) != 10U; }, CERBLIB_LOCATION);
        test(
            []() { return !std::is_same_v<decltype(abs<u32>(-10)), u32>; },
            CERBLIB_LOCATION);

        test([]() { return not_equal(abs(0.0f), 0.0f); }, CERBLIB_LOCATION);
        test([]() { return not_equal(abs(10.0f), 10.0f); }, CERBLIB_LOCATION);

        test([]() { return not_equal(abs(-0.0f), 0.0f); }, CERBLIB_LOCATION);
        test([]() { return not_equal(abs(-10.0f), 10.0f); }, CERBLIB_LOCATION);

        test([]() { return not_equal(abs(0.0f), 0.0f); }, CERBLIB_LOCATION);
        test([]() { return not_equal(abs(10.0f), 10.0f); }, CERBLIB_LOCATION);

        test([]() { return not_equal(abs(-0.0), 0.0); }, CERBLIB_LOCATION);
        test([]() { return not_equal(abs(-10.0), 10.0); }, CERBLIB_LOCATION);
    }

    auto bit_test() -> int
    {
        test_mask();
        test_min_max();
        test_pow2();
        test_abs();
        return 0;
    }
}// namespace cerb::test
