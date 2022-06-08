#include <cerberus/debug/debug.hpp>
#include <cerberus/enum.hpp>
#include <cerberus/lazy_executor.hpp>
#include <random>

namespace cerb::debug
{
    CERBERUS_ENUM(// NOLINTNEXTLINE
        LazyExecutorOptions, u16, DEFAULT = 0, REMOVE_THREAD = 1, ADD_THREAD = 2, JOIN = 4);

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
            auto call_test_function = [this]() { return testFunction(this); };

            for (size_t i = 0; i != tasks_number; ++i) {
                if (i == tasks_number / 8) {
                    applyOption(option);
                }

                executor.addJob(call_test_function);
            }

            finishJobs();
        }

        auto finishJobs() -> void
        {
            auto is_zero = [](int value) { return value == 0; };
            executor.stop();

            ASSERT_EQUAL(static_cast<int>(counter), tasks_number);
            ASSERT_TRUE(std::ranges::all_of(value_to_zero, is_zero));
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

            case LazyExecutorOptions::JOIN:
                executor.join();
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

            randomAction();

            return counter_value;
        }

        static auto randomAction() -> void
        {
            using namespace std::chrono_literals;

            if (distribution(engine) == 0) {
                std::this_thread::yield();
            } else {
                std::this_thread::sleep_for(1ms);
            }
        }

        static constexpr int tasks_number = 512;

        static inline std::random_device random_device{};
        static inline std::mt19937 engine{ random_device() };
        static inline std::uniform_int_distribution<u16> distribution{ 0, 1 };

        std::atomic<int> counter{ 0 };
        LazyExecutor<int> executor{ std::thread::hardware_concurrency() };
        std::array<int, tasks_number> value_to_zero{};
    };

    auto testLazyExecutor() -> int
    {
        LazyExecutorTester executor_test_default{ LazyExecutorOptions::DEFAULT };
        LazyExecutorTester executor_test_with_extra_thread{ LazyExecutorOptions::ADD_THREAD };
        LazyExecutorTester executor_test_with_removed_thread{ LazyExecutorOptions::REMOVE_THREAD };
        LazyExecutorTester executor_test_with_joined_thread{ LazyExecutorOptions::JOIN };

        return 0;
    }
}// namespace cerb::debug