#ifndef MTQUEUE_QUEUE_H
#define MTQUEUE_QUEUE_H

#include <array>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <optional>
#include <stop_token>

template <typename ElementType, std::size_t BufferSize> class queue {
  std::mutex mutex;
  std::condition_variable_any read_cv;
  std::condition_variable_any write_cv;
  std::stop_source closed;

  std::array<ElementType, BufferSize> buffer;
  std::size_t start{};
  std::size_t length{};

public:
  queue() = default;

  queue(const queue &) = delete;
  queue &operator=(const queue &) = delete;

  queue(queue &&) noexcept = delete;
  queue &operator=(queue &&) noexcept = delete;

  void push(ElementType element) {
    {
      std::unique_lock lock(mutex);

      if (!write_cv.wait(lock, closed.get_token(),
                         [this]() { return length < BufferSize; })) {
        return;
      }

      buffer[(start + length) % BufferSize] = std::move(element);
      length++;
    }

    write_cv.notify_one();
    read_cv.notify_one();
  }

  std::optional<ElementType> pop() {
    ElementType value;

    {
      std::unique_lock lock(mutex);

      read_cv.wait(lock, closed.get_token(), [this]() { return length > 0; });

      if (length == 0) {
        return std::nullopt;
      }

      value = std::move(buffer[start]);
      length--;
      start = (start + 1) % BufferSize;
    }

    read_cv.notify_one();
    write_cv.notify_one();

    return std::move(value);
  }

  void close() {
    closed.request_stop();

    write_cv.notify_all();
    read_cv.notify_all();
  }
};

#endif // MTQUEUE_QUEUE_H
