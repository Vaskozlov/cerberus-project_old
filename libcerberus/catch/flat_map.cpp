#include <cerberus/cerberus.hpp>
#include <cerberus/debug/debug.hpp>
#include <cerberus/flat_map.hpp>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testFlatMapOfInts)
    {
        // NOLINTBEGIN
        FlatMap<int, int, 4> flat_map{};
        flat_map.insert({ 10, 20 });

        EXPECT_EQUAL(flat_map[10], 20);
        EXPECT_EQUAL(flat_map[20], 0);
        EXPECT_EQUAL(std::as_const(flat_map)[20], 0);

        flat_map[20] = 30;
        EXPECT_EQUAL(flat_map[20], 30);

        flat_map.emplace(30, 40);
        EXPECT_EQUAL(flat_map[30], 40);

        flat_map.emplace(40, 50);
        EXPECT_EQUAL(flat_map.size(), 4);
        // NOLINTEND

        return true;
    }

    auto testFlatMapThrowOnFull() -> void
    {
        FlatMap<int, int, 1> flat_map{};
        flat_map.insert({ 0, 0 });

        try {
            flat_map.emplace(0, 0);
            CANT_BE_REACHED;
        } catch (std::out_of_range const &) {
            MUST_BE_REACHED;
        }
    }

    auto testFlatMap() -> int
    {
        CERBERUS_TEST(testFlatMapOfInts());
        testFlatMapThrowOnFull();
        return 0;
    }
}// namespace cerb::debug