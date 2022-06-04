#ifndef CERBERUS_LAZY_EXECUTOR_HPP
#define CERBERUS_LAZY_EXECUTOR_HPP

#include <any>
#include <cerberus/cerberus.hpp>
#include <cerberus/exception.hpp>
#include <cerberus/memory.hpp>
#include <deque>
#include <functional>
#include <list>
#include <mutex>
#include <semaphore>
#include <thread>

namespace cerb
{
    template<typename ReturnType = std::any>
    class LazyExecutor
    {
    private:
        using job_function_t = std::function<ReturnType()>;
        using completion_function_t = std::function<void(ReturnType)>;

        struct Action
        {
            job_function_t job;
            completion_function_t completion;
        };

    public:
        [[nodiscard]] auto threadsNumber() const -> size_t
        {
            return threads_storage.size();
        }

        auto addJob(Action action) -> void
        {
            std::scoped_lock lock{ queue_lock };
            actions_queue.push_back(std::move(action));
        }

        auto addJob(job_function_t job, completion_function_t completion = emptyFunction) -> void
        {
            addJob(Action{ job, completion });
        }

        auto addPriorityJob(Action action) -> void
        {
            std::scoped_lock lock{ queue_lock };
            actions_queue.push_front(std::move(action));
        }

        auto addPriorityJob(job_function_t job, completion_function_t completion = emptyFunction)
            -> void
        {
            addPriorityJob(Action{ job, completion });
        }

        auto addThread() -> void
        {
            threads_run_flag.emplace_back(true);
            threads_storage.emplace_back(threadLoop, this, &threads_run_flag.back());
        }

        auto removeThread() -> void
        {
            if (threads_storage.size() > 1) {
                auto &last_thread = threads_storage.back();

                threads_run_flag.back() = false;
                last_thread.join();

                threads_storage.pop_back();
                threads_run_flag.pop_back();
            }
        }

        auto stop() -> void
        {
            waitUntilQueueIsEmpty();
            std::scoped_lock lock{ queue_lock };
            joinAllRunningThreads();
        }

        auto join() -> void
        {
            stop();
            run = true;
            restartThreads();
        }

        auto operator=(LazyExecutor &&) -> LazyExecutor & = delete;
        auto operator=(LazyExecutor const &) -> LazyExecutor & = delete;

        LazyExecutor(LazyExecutor &&) = delete;
        LazyExecutor(LazyExecutor const &) = delete;

        explicit LazyExecutor(size_t threads_count = 4)
        {
            for (size_t i = 0; i < threads_count; ++i) {
                addThread();
            }
        }

        ~LazyExecutor()
        {
            stop();
        }

    private:
        static auto threadLoop(LazyExecutor *executor, bool const *run_flag) -> void
        {
            while (logicalAnd(executor->run, *run_flag)) {
                tryToGrabAndExecuteTask(executor);
            }
        }

        static auto tryToGrabAndExecuteTask(LazyExecutor *executor) -> void
        {
            using namespace std::chrono_literals;

            Action action{};

            if (tryToGrabAction(executor, action)) {
                executeTask(action);
            } else {
                std::this_thread::sleep_for(20ms);
            }
        }

        static auto tryToGrabAction(LazyExecutor *executor, Action &action) -> bool
        {
            std::deque<Action> &queue = executor->actions_queue;
            std::scoped_lock lock{ executor->queue_lock };

            if (not queue.empty()) {
                action = queue.front();
                queue.pop_front();

                return true;
            }

            return false;
        }

        static auto executeTask(Action action) -> void
        {
            action.completion(action.job());
        }

        auto waitUntilQueueIsEmpty() -> void
        {
            using namespace std::chrono_literals;

            while (not actions_queue.empty()) {
                std::this_thread::sleep_for(5ms);
            }
        }

        auto restartThreads() -> void
        {
            auto run_flag = threads_run_flag.begin();

            for (std::jthread &thread : threads_storage) {
                thread = std::jthread(threadLoop, this, std::to_address(run_flag));
                ++run_flag;
            }
        }

        auto joinAllRunningThreads() -> void
        {
            run = false;

            for (std::jthread &thread : threads_storage) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        }

        static auto emptyFunction(ReturnType /*unused*/) -> void
        {
            // empty function for completion
        }

        std::deque<Action> actions_queue{};
        std::list<std::jthread> threads_storage{};
        std::list<bool> threads_run_flag;
        std::mutex queue_lock{};
        bool run{ true };
    };
}// namespace cerb

#endif /* CERBERUS_LAZY_EXECUTOR_HPP */
