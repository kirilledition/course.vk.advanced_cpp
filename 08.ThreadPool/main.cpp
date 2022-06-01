#include <cstddef>
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  explicit ThreadPool(size_t poolSize) : flag_(true) {
    for (size_t i = 0; i < poolSize; ++i) {
      threads_.emplace_back([this]() {
        while (flag_) {
          std::unique_lock<std::mutex> lock(mutex_);
          if (!taskQueue_.empty()) {
            auto task = taskQueue_.front();
            taskQueue_.pop();
            lock.unlock();
            task();
          } else {
            hasTask_.wait(lock);
          }
        }
      });
    }
  };
  ~ThreadPool() {
    flag_ = false;
    hasTask_.notify_all();
    for (auto& thread : threads_) {
      thread.join();
    }
  };
  template <class Func, class... Args>
  auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
    using typeReturn = decltype(func(args...));
    using packagedFunc = std::packaged_task<typeReturn()>;
    // using packagedFunc = std::packaged_task<decltype(func(args...))>;
    auto task = std::make_shared<packagedFunc>(
        [func, args...]() { return func(args...); });
    {
      std::lock_guard<std::mutex> lock(mutex_);
      taskQueue_.emplace([task]() { (*task)(); });
    }
    hasTask_.notify_one();
    return task->get_future();
  }

  std::atomic<bool> flag_;
  std::vector<std::thread> threads_;
  std::queue<std::function<void()>> taskQueue_;
  std::mutex mutex_;
  std::condition_variable hasTask_;
};
