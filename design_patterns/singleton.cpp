#include <iostream>

struct logger {
    static logger& instance() {
        static logger instance;
        return instance;
    }

    logger(const logger&) = delete;

    logger& operator=(const logger&) = delete;

    void log(const std::string& message) const {
        std::cout << "logging: " << message << std::endl;
    }

  private:
    logger() { std::cout << "Initializing instance" << std::endl; };

    ~logger() = default;
};

int main() {
    logger::instance().log("Hello");
    logger::instance().log("Hi back");
}
