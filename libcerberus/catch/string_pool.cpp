#include "string_pool.hpp"
#include <cerberus/string_pool.hpp>

namespace cerb::test {
    auto stringPoolTest() -> int
    {
        StringPool<char, unsigned> string_pool({ { "Hello", 0 }, { "World", 1 } });

        EXPECT_TRUE(string_pool.contains("Hello"));
        EXPECT_TRUE(string_pool.at("Hello") == 0);
        EXPECT_TRUE(string_pool.at("World") == 1);
        EXPECT_FALSE(string_pool.contains("Test"));

        string_pool.try_emplace("Test", 3);
        EXPECT_TRUE(string_pool.contains("Test"));
        EXPECT_TRUE(string_pool.at("Test") == 3);

        return 0;
    }
}// namespace cerb::test
