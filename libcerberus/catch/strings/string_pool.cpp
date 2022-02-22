#include "string_pool.hpp"
#include "cerberus/string_pool.hpp"

namespace cerb::debug
{
    auto stringPoolTest() -> int
    {
        StringPool<char, unsigned> string_pool({ { "Hello", 0 }, { "World", 1 } });

        EXPECT_TRUE(string_pool.contains("Hello"));
        EXPECT_TRUE(string_pool["Hello"] == 0);
        EXPECT_TRUE(string_pool["World"] == 1);
        EXPECT_TRUE(string_pool.findLongestStringInPool("WorldIt'sATest") == "World");

        try {
            string_pool["Test"];
            EXPECT_FALSE(1);
        } catch (std::out_of_range const &) {
            EXPECT_TRUE(1);
        }

        string_pool.emplace("Test", 3);
        EXPECT_TRUE(string_pool.contains("Test"));
        EXPECT_TRUE(string_pool["Test"] == 3);

        return 0;
    }
}// namespace cerb::debug
