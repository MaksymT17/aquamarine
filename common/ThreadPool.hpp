#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
    public:
    //according to test on CPU with Hyper-threading feature better to use mutiplier of 2
    // without hyper-threading - probably this mutiplier shall be 1, didn`t test yet
    ThreadPool(unsigned num_threads = std::thread::hardware_concurrency()*2) {
        while (num_threads--) {
            threads.emplace_back([this] {
                while(true) {
                    std::unique_lock<std::mutex> lock(mutex);
                    condvar.wait(lock, [this] {return !queue.empty();});
                    auto task = std::move(queue.front());
                    if (task.valid()) {
                        queue.pop();
                        lock.unlock();
                        // run the task - this cannot throw; any exception
                        // will be stored in the corresponding future
                        task();
                    } else {
                        // an empty task is used to signal end of stream
                        // don't pop it off the top; all threads need to see it
                        break;
                    }
                }
            });
        }
    }

    template<typename F, typename R = std::result_of_t<F&&()>>
    std::future<R> run(F&& f) const {
        auto task = std::packaged_task<R()>(std::forward<F>(f));
        auto future = task.get_future();
        {
            std::lock_guard<std::mutex> lock(mutex);
            // conversion to packaged_task<void()> erases the return type
            // so it can be stored in the queue. the future will still
            // contain the correct type
            queue.push(std::packaged_task<void()>(std::move(task)));
        }
        condvar.notify_one();
        return future;
    }

    ~ThreadPool() {
        // push a single empty task onto the queue and notify all threads,
        // then wait for them to terminate
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push({});
        }
        condvar.notify_all();
        for (auto& thread : threads) {
            thread.join();
        }
    }

    private:
    std::vector<std::thread> threads;
    mutable std::queue<std::packaged_task<void()>> queue;
    mutable std::mutex mutex;
    mutable std::condition_variable condvar;
};