#include <cerberus/bitmap.hpp>
#include <cerberus/debug/debug.hpp>

namespace cerb::debug
{
    using namespace bit;

    CERBLIB_DECL auto testBitMapSet() -> bool
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

        return true;
    }

    CERBLIB_DECL auto testBitMapAt() -> bool
    {
        ConstBitmap<2, 512> bitmap;

        bitmap.set<1, 0>(10);
        EXPECT_TRUE(bitmap.at<0>(10) == 1);
        EXPECT_TRUE(bitmap.at<0>(20) == 0);

        bitmap.set<1, 1>(20);
        EXPECT_TRUE(bitmap.at<1>(20) == 1);
        EXPECT_TRUE(bitmap.at<1>(19) == 0);

        return true;
    }

    CERBLIB_DECL auto testBitMapClear() -> bool
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

        return true;
    }

    CERBLIB_DECL auto testBitMapFind() -> bool
    {
        ConstBitmap<3, 512> bitmap;

        bitmap.set<1, 0>(100);
        bitmap.set<1, 2>(100);

        EXPECT_TRUE(safeEqual<size_t>(
            bitmap.find<ValueOfBit::ONE, ValueOfBit::ZERO, ValueOfBit::ONE>(), 100));

        bitmap.set<1, 2>(64);

        EXPECT_TRUE(safeEqual<size_t>(
            bitmap.find<ValueOfBit::ZERO, ValueOfBit::ANY, ValueOfBit::ONE>(), 64));

        EXPECT_TRUE(safeEqual(
            bitmap.find<ValueOfBit::ONE, ValueOfBit::ONE, ValueOfBit::ONE>(), bitmap.npos));

        return true;
    }

    auto testBitmap() -> int
    {
        CR_EXPECT_TRUE(testBitMapSet());
        CR_EXPECT_TRUE(testBitMapAt());
        CR_EXPECT_TRUE(testBitMapClear());
        CR_EXPECT_TRUE(testBitMapFind());
        return 0;
    }
}// namespace cerb::debug
