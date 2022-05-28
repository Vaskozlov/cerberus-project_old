#include <cerberus/bitmap.hpp>
#include <cerberus/debug/debug.hpp>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_VECTOR(testBitmapSetAndAt)
    {
        Bitmap bitmap{};

        // NOLINTBEGIN

        bitmap.set<1>(10);
        ASSERT_TRUE(bitmap.at(10));

        bitmap.set<0>(10);
        ASSERT_FALSE(bitmap.at(10));

        bitmap.set<1>(127);
        ASSERT_TRUE(bitmap.at(127));

        bitmap.set<1>(128);
        ASSERT_TRUE(bitmap.at(128));

        bitmap.set<1>(255);
        ASSERT_TRUE(bitmap.at(255));

        bitmap.set<1>(256);
        ASSERT_TRUE(bitmap.at(256));

        bitmap.set<1>(1023);
        ASSERT_TRUE(bitmap.at(1023));

        bitmap.set<1>(1024);
        ASSERT_TRUE(bitmap.at(1024));

        // NOLINTEND

        return true;
    }

    auto testBitmap() -> int
    {
        CERBERUS_TEST_FOR_CONSTEXPR_VECTOR(testBitmapSetAndAt());
        return 0;
    }
}// namespace cerb::debug
