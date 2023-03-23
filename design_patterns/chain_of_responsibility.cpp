#include <iostream>
#include <memory>

class request {
    bool authenticated;

  public:
    explicit request(bool authenticated) : authenticated{authenticated} {}

    [[nodiscard]] bool is_authenticated() const { return authenticated; }
};

class response {
  public:
    virtual ~response() = default;

    virtual void describe() const = 0;
};

class unauthenticated_response : public response {
    void describe() const override {
        std::cout << "Unauthenticated." << std::endl;
    }
};

class success_response : public response {
    void describe() const override { std::cout << "Success!" << std::endl; }
};

class request_handler {
  public:
    virtual ~request_handler() = default;

    // Assuming the request handlers do not take ownership of each other.
    virtual void set_next(request_handler* next_handler) = 0;

    [[nodiscard]] virtual std::unique_ptr<response>
    handle(const request&) const = 0;
};

class base_request_handler : public request_handler {
    request_handler* next{};

  public:
    void set_next(request_handler* next_handler) final { next = next_handler; }

    [[nodiscard]] const request_handler* get_next() const { return next; }
};

class authentication_handler : public base_request_handler {
  public:
    [[nodiscard]] std::unique_ptr<response>
    handle(const request& request) const final {
        if (request.is_authenticated()) {
            if (auto next = get_next()) {
                return next->handle(request);
            }
            throw std::runtime_error(
                "Authentication handler requires a next handler.");
        }
        return std::make_unique<unauthenticated_response>();
    }
};

class file_request_handler : public base_request_handler {
  public:
    [[nodiscard]] std::unique_ptr<response> handle(const request&) const final {
        return std::make_unique<success_response>();
    }
};

int main() {
    authentication_handler first_handler;
    file_request_handler second_handler;

    first_handler.set_next(&second_handler);

    first_handler.handle(request{true})->describe();
    first_handler.handle(request{false})->describe();
}
