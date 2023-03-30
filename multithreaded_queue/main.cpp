#include <fstream>
#include <thread>
#include <vector>

#include "queue.h"

template <typename T>
concept writeable = requires(T t, std::ostream f) {
  { f << t } -> std::same_as<std::ostream &>;
};

template <typename Clock> class logger {
  std::ofstream os;
  typename Clock::time_point start;
  std::string name;

public:
  logger(std::ofstream os, typename Clock::time_point start, std::string name)
      : os{std::move(os)}, start{start}, name{std::move(name)} {}

  template <writeable T> void write(const T &s) {
    os << "[" << (Clock::now() - start).count() << "][" << name << "]: " << s
       << std::endl;
  }
};

template <typename TimePoint>
logger(std::ofstream, TimePoint, std::string)
    -> logger<typename TimePoint::clock>;

int main() {
  constexpr std::size_t n_messages_per_thread{10};
  constexpr std::size_t n_producers{2};
  constexpr std::size_t n_consumers{3};
  const auto start = std::chrono::high_resolution_clock::now();

  queue<std::pair<std::size_t, std::size_t>, 1> queue;

  std::vector<std::thread> consumers;
  for (std::size_t i = 0; i < n_consumers; ++i) {
    consumers.emplace_back([i, start, &queue] {
      logger log{
          std::ofstream("consumer-" + std::to_string(i) + ".txt"),
          start,
          "consumer-" + std::to_string(i),
      };

      log.write("ready");

      bool queue_closed = false;

      while (!queue_closed) {
        auto value = queue.pop();

        if (value) {
          log.write("received " + std::to_string(value->second) + " from " +
                    std::to_string(value->first));
        } else {
          queue_closed = true;
        }
      }

      log.write("closed");
    });
  }

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1s);

  std::vector<std::thread> producers;
  for (std::size_t i = 0; i < n_producers; ++i) {
    producers.emplace_back([i, start, &queue] {
      logger log{
          std::ofstream("producer-" + std::to_string(i) + ".txt"),
          start,
          "producer-" + std::to_string(i),
      };

      for (std::size_t m = 0; m < n_messages_per_thread; ++m) {
        log.write("pushing " + std::to_string(m));
        queue.push({i, m});
        log.write("pushed " + std::to_string(m));
      }
    });
  }

  for (auto &thread : producers)
    thread.join();

  queue.close();

  for (auto &thread : consumers)
    thread.join();

  return 0;
}
