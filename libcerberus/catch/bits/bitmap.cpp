#include "bitmap.hpp"
#include <cerberus/bitmap.hpp>

namespace cerb::debug
{
    auto bitmapTestSet() -> void
    {
        ConstBitmap<2, 512> bitmap;
        bitmap.set<1, 0>(10);

        EXPECT_TRUE(bitmap.data()[0][0] == 1024);

        bitmap.set<1, 1>(20);
        EXPECT_TRUE(bitmap.data()[1][0] == (1 << 20));

        bitmap.set<0, 0>(10);
        EXPECT_TRUE(bitmap.data()[0][0] == 0);

        bitmap.set<0, 1>(20);
        EXPECT_TRUE(bitmap.data()[1][0] == 0);
    }

    auto bitmapTestAt() -> void
    {
        ConstBitmap<2, 512> bitmap;

        bitmap.set<1, 0>(10);
        EXPECT_TRUE(bitmap.at<0>(10) == 1);
        EXPECT_TRUE(bitmap.at<0>(20) == 0);

        bitmap.set<1, 1>(20);
        EXPECT_TRUE(bitmap.at<1>(20) == 1);
        EXPECT_TRUE(bitmap.at<1>(19) == 0);
    }

    auto bitmapTestClear() -> void
    {
        ConstBitmap<2, 512> bitmap;
        auto const &bitmap_data = bitmap.data();
        auto const bitmap_data_begin = bitmap_data.begin();
        auto is_zero = [](auto elem) { return elem == 0; };

        bitmap.set<1, 0>(0);
        bitmap.set<1, 1>(0);
        bitmap.set<1, 0>(511);
        bitmap.set<1, 1>(511);

        bitmap.clear<0>();
        EXPECT_TRUE(std::ranges::all_of(*bitmap_data_begin, is_zero));

        bitmap.set<1, 0>(0);
        bitmap.set<1, 0>(511);

        bitmap.clear();

        EXPECT_TRUE(std::ranges::all_of(bitmap_data, [&is_zero](auto const &array) {
            return std::ranges::all_of(array, is_zero);
        }));
    }

    auto bitmapTestSimpleFind() -> void
    {
        ConstBitmap<3, 512> bitmap;

        bitmap.set<1, 0>(100);
        bitmap.set<1, 2>(100);

        auto test_find_1 =
            bitmap.find<bit::ValueOfBit::ONE, bit::ValueOfBit::ZERO, bit::ValueOfBit::ONE>();
        EXPECT_TRUE(test_find_1 == 100);

        bitmap.set<1, 2>(64);

        auto test_find_2 =
            bitmap.find<bit::ValueOfBit::ZERO, bit::ValueOfBit::ANY, bit::ValueOfBit::ONE>();
        EXPECT_TRUE(test_find_2 == 64);

        auto test_find_3 =
            bitmap.find<bit::ValueOfBit::ONE, bit::ValueOfBit::ONE, bit::ValueOfBit::ONE>();
        EXPECT_TRUE(test_find_3 == bitmap.npos);
    }

    auto bitmapTest(u32) -> int
    {
        bitmapTestSet();
        bitmapTestAt();
        bitmapTestClear();
        bitmapTestSimpleFind();
        return 0;
    }
}// namespace cerb::test
