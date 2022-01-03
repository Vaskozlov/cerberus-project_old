#include "bitmap.hpp"
#include <cerberus/bitmap.hpp>

namespace cerb::test {
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
        const auto &bitmap_data = bitmap.data();
        const auto bitmap_data_begin = bitmap_data.begin();

        bitmap.set<1, 0>(0);
        bitmap.set<1, 1>(0);
        bitmap.set<1, 0>(511);
        bitmap.set<1, 1>(511);

        bitmap.clear<0>();
        EXPECT_TRUE(
            std::all_of(bitmap_data_begin->begin(), bitmap_data_begin->end(), [](auto elem) {
                return elem == 0;
            }));

        bitmap.set<1, 0>(0);
        bitmap.set<1, 0>(511);

        bitmap.clear();

        EXPECT_TRUE(std::all_of(bitmap_data.begin(), bitmap_data.end(), [](const auto &array) {
            return std::all_of(array.begin(), array.end(), [](auto elem) { return elem == 0; });
        }));
    }

    auto bitmapTestSimpleFind() -> void
    {
        ConstBitmap<3, 512> bitmap;

        bitmap.set<1, 0>(100);
        bitmap.set<1, 2>(100);

        auto position_1 =
            bitmap.find<bit::ValueOfBit::ONE, bit::ValueOfBit::ZERO, bit::ValueOfBit::ONE>();
        EXPECT_TRUE(position_1 == 100);

        bitmap.set<1, 2>(64);

        auto position_2 =
            bitmap.find<bit::ValueOfBit::ZERO, bit::ValueOfBit::ANY, bit::ValueOfBit::ONE>();
        EXPECT_TRUE(position_2 == 64);

        auto position_3 =
            bitmap.find<bit::ValueOfBit::ONE, bit::ValueOfBit::ONE, bit::ValueOfBit::ONE>();
        EXPECT_TRUE(position_3 == bitmap.npos);
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
