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

        ASSERT_EQUAL(flat_map[10], 20);
        ASSERT_EQUAL(flat_map[20], 0);
        ASSERT_EQUAL(std::as_const(flat_map)[20], 0);

        flat_map[20] = 30;
        ASSERT_EQUAL(flat_map[20], 30);

        flat_map.emplace(30, 40);
        ASSERT_EQUAL(flat_map[30], 40);

        flat_map.emplace(40, 50);
        ASSERT_EQUAL(flat_map.size(), 4);
        // NOLINTEND

        return true;
    }

    auto testFlatMapThrowOnFull() -> void
    {
        FlatMap<int, int, 1> flat_map{};
        flat_map.insert({ 0, 0 });

        ERROR_EXPECTED(flat_map.emplace(0, 0), std::out_of_range, "Cerberus flat map is full!")
    }

    auto testFlatMap() -> int
    {
        CERBERUS_TEST(testFlatMapOfInts());
        testFlatMapThrowOnFull();
        return 0;
    }
}// namespace cerb::debug