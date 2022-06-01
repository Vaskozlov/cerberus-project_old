#include <cerberus/debug/debug.hpp>
#include <cerberus/enum.hpp>
#include <cerberus/lazy_executor.hpp>

namespace cerb::debug
{
    // NOLINTNEXTLINE
    CERBERUS_ENUM(LazyExecutorOptions, u16, DEFAULT = 0, REMOVE_THREAD = 1, ADD_THREAD = 2);

    struct LazyExecutorTester
    {
        LazyExecutorTester() = default;

        explicit LazyExecutorTester(LazyExecutorOptions option)
        {
            fillTestArray();
            startJobs(option);
        }

    private:
        auto startJobs(LazyExecutorOptions option) -> void
        {
            for (size_t i = 0; i != tasks_number; ++i) {
                if (i == tasks_number / 4) {
                    applyOption(option);
                }

                executor.addJob([this]() { return testFunction(this); });
            }

            endJobs();
        }

        auto endJobs() -> void
        {
            executor.join();
            ASSERT_EQUAL(static_cast<int>(counter), tasks_number);
            ASSERT_TRUE(std::ranges::all_of(value_to_zero, [](int value) { return value == 0; }));
            return;
        }

        auto applyOption(LazyExecutorOptions option) -> void
        {
            switch (option.get()) {
            case LazyExecutorOptions::ADD_THREAD:
                executor.addThread();
                break;

            case LazyExecutorOptions::REMOVE_THREAD:
                executor.removeThread();
                break;

            default:
                break;
            }
        }

        constexpr auto fillTestArray() -> void
        {
            int i = 0;

            for (int &elem : value_to_zero) {
                elem = i;
                ++i;
            }
        }

        static auto testFunction(LazyExecutorTester *tester) -> int
        {
            int counter_value = tester->counter++;
            tester->value_to_zero[static_cast<uint>(counter_value)] = 0;// NOLINT
            return counter_value;
        }

        static constexpr int tasks_number = 128;

        std::atomic<int> counter{ 0 };
        LazyExecutor<int> executor{ std::thread::hardware_concurrency() };
        std::array<int, tasks_number> value_to_zero{};
    };

    auto testLazyExecutor() -> int
    {
        LazyExecutorTester executor_test_default{ LazyExecutorOptions::DEFAULT };
        LazyExecutorTester executor_test_with_extra_thread{ LazyExecutorOptions::ADD_THREAD };
        LazyExecutorTester executor_test_with_removed_thread{ LazyExecutorOptions::REMOVE_THREAD };
        return 0;
    }
}// namespace cerb::debug