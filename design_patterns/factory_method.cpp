#include <iostream>
#include <memory>

class transport {
  public:
    virtual ~transport() = default;

    virtual void deliver() = 0;
};

class truck : public transport {
  public:
    void deliver() override { std::cout << "Delivering by truck" << std::endl; }
};

class ship : public transport {
  public:
    void deliver() override { std::cout << "Delivering by ship" << std::endl; }
};

class logistics {
  public:
    virtual ~logistics() = default;

    [[nodiscard]] virtual std::unique_ptr<transport>
    create_transport() const = 0;

    void plan_delivery() const {
        auto transport = this->create_transport();

        transport->deliver();
    }
};

class road_logistics : public logistics {
  public:
    [[nodiscard]] std::unique_ptr<transport> create_transport() const override {
        return std::make_unique<truck>();
    }
};

class sea_logistics : public logistics {
  public:
    [[nodiscard]] std::unique_ptr<transport> create_transport() const override {
        return std::make_unique<ship>();
    }
};

void plan_delivery(const logistics& logistics) { logistics.plan_delivery(); }

int main() {
    plan_delivery(road_logistics{});

    plan_delivery(sea_logistics{});
}
