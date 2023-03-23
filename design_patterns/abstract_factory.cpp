#include <iostream>
#include <memory>

class chair {
  public:
    virtual ~chair() = default;

    virtual void describe() const = 0;
};

class victorian_chair : public chair {
  public:
    void describe() const override {
        std::cout << "A victorian chair" << std::endl;
    }
};

class modern_chair : public chair {
  public:
    void describe() const override {
        std::cout << "A modern chair" << std::endl;
    }
};

class table {
  public:
    virtual ~table() = default;

    virtual void describe() const = 0;
};

class victorian_table : public table {
  public:
    void describe() const override {
        std::cout << "A victorian table" << std::endl;
    }
};

class modern_table : public table {
  public:
    void describe() const override {
        std::cout << "A modern table" << std::endl;
    }
};

class furniture_factory {
  public:
    virtual ~furniture_factory() = default;

    [[nodiscard]] virtual std::unique_ptr<chair> create_chair() const = 0;

    [[nodiscard]] virtual std::unique_ptr<table> create_table() const = 0;
};

class victorian_factory : public furniture_factory {
  public:
    [[nodiscard]] std::unique_ptr<chair> create_chair() const override {
        return std::make_unique<victorian_chair>();
    }

    [[nodiscard]] std::unique_ptr<table> create_table() const override {
        return std::make_unique<victorian_table>();
    }
};

class modern_factory : public furniture_factory {
  public:
    [[nodiscard]] std::unique_ptr<chair> create_chair() const override {
        return std::make_unique<modern_chair>();
    }

    [[nodiscard]] std::unique_ptr<table> create_table() const override {
        return std::make_unique<modern_table>();
    }
};

void create_living_room(const furniture_factory& factory) {
    auto chair = factory.create_chair();
    chair->describe();

    auto table = factory.create_table();
    table->describe();
}

int main() {
    create_living_room(victorian_factory{});

    create_living_room(modern_factory{});
}
