#include "flat_map.hpp"
#include <cerberus/cerberus.hpp>
#include <cerberus/flat_map.hpp>

namespace cerb::debug
{
    auto flatMapTest(u32) -> int
    {
        FlatMap<int, int, 4> flat_map{};
        flat_map.insert({ 10, 20 });

        EXPECT_TRUE(flat_map[10] == 20);
        EXPECT_TRUE(flat_map[20] == 0);
        EXPECT_TRUE(std::as_const(flat_map)[20] == 0);

        flat_map[20] = 30;
        EXPECT_TRUE(flat_map[20] == 30);

        flat_map.emplace(30, 40);
        EXPECT_TRUE(flat_map[30] == 40);

        flat_map.emplace(40, 50);
        EXPECT_TRUE(flat_map.size() == 4);

        try {
            flat_map.emplace(50, 60);
            EXPECT_FALSE(1);
        } catch (std::out_of_range const &) {
            EXPECT_TRUE(1);
        }

        return 0;
    }
}// namespace cerb::debug