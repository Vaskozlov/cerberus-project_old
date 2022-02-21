#include "bit.hpp"
#include <cerberus/number.hpp>

namespace cerb::debug
{

    auto byteMaskTest() -> void
    {
        ByteMask mask{ 0ll };
        mask.mask_8[0] = 255;
        mask.mask_8[2] = 255;

        EXPECT_TRUE(mask.getAsInt() == 0xFF00FF);

        mask.getAsInt() += 0xFF00;

        EXPECT_TRUE(mask.getAsInt() == 0xFFFFFF);
    }

    auto minMaxTest() -> void
    {
        int v_min = min<int>(10, -41, 14515, 0);
        int v_max = max<int>(10, -41, 14515, 0);

        EXPECT_TRUE(v_min == -41);
        EXPECT_TRUE(v_max == 14515);
    }

    auto forEachTest() -> void
    {
        int counter = 0;

        EXPECT_FALSE([&counter]() {
            forEach(
                [&counter](int elem) {
                    EXPECT_TRUE(elem == counter * 10);
                    ++counter;
                },
                0,
                10,
                20,
                30,
                40,
                50);
            return 0;
        }());
    }

    auto pow2Test() -> void
    {
        EXPECT_TRUE(pow2<u32>(0U) == 1U);
        EXPECT_TRUE(pow2<u32>(2U) == 4U);
        EXPECT_TRUE(pow2<u32>(10U) == 1024U);
        EXPECT_TRUE(pow2<u64>(54ULL) == (1ULL << 54ULL));

        EXPECT_TRUE(safeEqual<f32>(pow2<f32>(0U), 1.0f));
        EXPECT_TRUE(safeEqual<f32>(pow2<f32>(2U), 4.0f));
        EXPECT_TRUE(safeEqual<f32>(pow2<f32>(10U), 1024.0f));
        EXPECT_TRUE(safeEqual<f32>(pow2<f32>(54ULL), static_cast<f32>((1ULL << 54ULL))));

        EXPECT_TRUE(safeEqual<f64>(pow2<f64>(0U), 1.0));
        EXPECT_TRUE(safeEqual<f64>(pow2<f64>(2U), 4.0));
        EXPECT_TRUE(safeEqual<f64>(pow2<f64>(10U), 1024.0));
        EXPECT_TRUE(safeEqual<f64>(pow2<f64>(54ULL), static_cast<f64>((1ULL << 54ULL))));
    }

    auto absTest() -> void
    {
        EXPECT_TRUE(abs(0) == 0);
        EXPECT_TRUE(abs(10) == 10);

        EXPECT_TRUE(abs(-10) == 10);
        EXPECT_TRUE(abs(-10) == 10U);

        EXPECT_TRUE(safeEqual<f32>(abs(0.0f), 0.0f));
        EXPECT_TRUE(safeEqual<f32>(abs(10.0f), 10.0f));

        EXPECT_TRUE(safeEqual<f32>(abs(-0.0f), 0.0f));
        EXPECT_TRUE(safeEqual<f32>(abs(-10.0f), 10.0f));

        EXPECT_TRUE(safeEqual<f32>(abs(0.0f), 0.0f));
        EXPECT_TRUE(safeEqual<f32>(abs(10.0f), 10.0f));

        EXPECT_TRUE(safeEqual<f64>(abs(-0.0), 0.0));
        EXPECT_TRUE(safeEqual<f64>(abs(-10.0), 10.0));
    }

    auto bitScanTest(u32 argc) -> void
    {
        EXPECT_TRUE(bit::scanForward<1>(0b1000U * argc) == 3);
        EXPECT_TRUE(bit::scanForward<1>(0b1001U * argc) == 0);
        EXPECT_TRUE(bit::scanReverse<1>(0b1000U * argc) == 3);
        EXPECT_TRUE(bit::scanReverse<1>(0b1001U * argc) == 3);

        EXPECT_TRUE([]() {
            constexpr auto value = bit::scanForward<1>(0b1000U);
            return value == 3;
        }());

        EXPECT_TRUE([]() {
            constexpr auto value = bit::scanForward<1>(0b1001U);
            return value == 0;
        }());

        EXPECT_TRUE([]() {
            constexpr auto value = bit::scanReverse<1>(0b1000U);
            return value == 3;
        }());

        EXPECT_TRUE([]() {
            constexpr auto value = bit::scanReverse<1>(0b1001U);
            return value == 3;
        }());
    }

    auto log2Test(u32 argc) -> void
    {
        EXPECT_TRUE(cerb::log2(8U * argc) == 3);
        EXPECT_TRUE(cerb::log2(1U * argc) == 0);

        EXPECT_TRUE([]() {
            constexpr auto value = cerb::log2(8U);
            return value == 3;
        }());

        EXPECT_TRUE([]() {
            constexpr auto value = cerb::log2(1U);
            return value == 0;
        }());

        EXPECT_TRUE(cerb::log2(8.0f * static_cast<float>(argc)) == 3);
        EXPECT_TRUE(cerb::log2(1.0f * static_cast<float>(argc)) == 0);

        EXPECT_TRUE([]() {
            constexpr auto value = cerb::log2(8.0f);
            return value == 3;
        }());

        EXPECT_TRUE([]() {
            constexpr auto value = cerb::log2(1.0f);
            return value == 0;
        }());

        EXPECT_TRUE([]() {
            constexpr auto value = cerb::log2(-8.0f);
            return value == -1;
        }());
    }

    auto bitTest(u32 argc) -> int
    {
        byteMaskTest();
        minMaxTest();
        pow2Test();
        forEachTest();
        absTest();
        bitScanTest(argc);
        log2Test(argc);
        return 0;
    }
}// namespace cerb::test
