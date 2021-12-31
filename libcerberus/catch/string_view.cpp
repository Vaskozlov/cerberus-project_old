#include "string_view.hpp"
#include <string>

namespace cerb::test {

    auto basicStringViewTestEqualStrings(u32 argc) -> void
    {
        std::string_view standard_str_view = "Hello, World!";
        cerb::string_view str_view         = "Hello, World!";

        EXPECT_TRUE(str_view == standard_str_view);
        EXPECT_FALSE(str_view != standard_str_view);
        EXPECT_TRUE(str_view >= standard_str_view);
        EXPECT_TRUE(str_view <= standard_str_view);
        EXPECT_FALSE(str_view > standard_str_view);
        EXPECT_FALSE(str_view < standard_str_view);

        EXPECT_TRUE(str_view.strView() == standard_str_view);
        EXPECT_TRUE(str_view.str() <= std::string(standard_str_view));

        EXPECT_TRUE(str_view.back() == standard_str_view.back());
        EXPECT_TRUE(str_view.front() == standard_str_view.front());
        EXPECT_TRUE(str_view.at(1 + argc) == standard_str_view.at(1 + argc));
        EXPECT_TRUE(str_view.find('!') == standard_str_view.find('!'));
        EXPECT_TRUE([&]() {
            for (size_t i = 0; i < standard_str_view.size(); ++i) {
                if (str_view[i] != standard_str_view[i]) {
                    return false;
                }
            }
            return true;
        }());

        EXPECT_TRUE([&]() {
            auto str_view_begin = str_view.cbegin();

            for (const char chr : standard_str_view) {
                if (*str_view_begin != chr) {
                    return false;
                }
                ++str_view_begin;
            }
            return true;
        }());
    }

    auto basicStringViewTestNotEqualStrings(u32) -> void
    {
        std::string_view standard_str_view = "It's a \"Hello, World!\" program";
        cerb::string_view str_view         = "It's not a \"Hello, World!\" program";

        EXPECT_FALSE(str_view == standard_str_view);
        EXPECT_TRUE(str_view != standard_str_view);
        EXPECT_TRUE(str_view >= standard_str_view);
        EXPECT_FALSE(str_view <= standard_str_view);
        EXPECT_TRUE(str_view > standard_str_view);
        EXPECT_FALSE(str_view < standard_str_view);

        EXPECT_TRUE(str_view.back() == standard_str_view.back());
        EXPECT_TRUE(str_view.front() == standard_str_view.front());
        EXPECT_TRUE(str_view.find('!') != standard_str_view.find('!'));
    }

    auto stringViewTest(u32 argc) -> int
    {
        basicStringViewTestEqualStrings(argc);
        basicStringViewTestNotEqualStrings(argc);
        return 0;
    }
}// namespace cerb::test