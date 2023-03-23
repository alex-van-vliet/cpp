#include <iostream>
#include <memory>

class house {
  public:
    virtual ~house() = default;

    virtual void describe() const = 0;
};

class medieval_house : public house {
    std::size_t windows{};
    std::size_t rooms{};

  public:
    medieval_house(std::size_t windows, std::size_t rooms)
        : windows{windows}, rooms{rooms} {}

    void describe() const override {
        std::cout << "A medieval house with " << windows << " windows and "
                  << rooms << " rooms" << std::endl;
    }
};

class modern_house : public house {
    std::size_t windows{};
    std::size_t rooms{};

  public:
    modern_house(std::size_t windows, std::size_t rooms)
        : windows{windows}, rooms{rooms} {}

    void describe() const override {
        std::cout << "A modern house with " << windows << " windows and "
                  << rooms << " rooms" << std::endl;
    }
};

class builder {
  public:
    virtual ~builder() = default;

    virtual builder& set_windows(std::size_t windows) = 0;

    virtual builder& set_rooms(std::size_t rooms) = 0;

    virtual void reset() = 0;

    [[nodiscard]] virtual std::unique_ptr<house> get_result() = 0;
};

class medieval_builder : public builder {
    std::size_t windows{0};
    std::size_t rooms{0};

  public:
    builder& set_windows(std::size_t windows) override {
        this->windows = windows;
        return *this;
    }

    builder& set_rooms(std::size_t rooms) override {
        this->rooms = rooms;
        return *this;
    }

    void reset() override {
        windows = 0;
        rooms = 0;
    }

    [[nodiscard]] std::unique_ptr<house> get_result() override {
        auto ptr = std::make_unique<medieval_house>(windows, rooms);

        reset();

        return ptr;
    }
};

class modern_builder : public builder {
    std::size_t windows{0};
    std::size_t rooms{0};

  public:
    builder& set_windows(std::size_t windows) override {
        this->windows = windows;
        return *this;
    }

    builder& set_rooms(std::size_t rooms) override {
        this->rooms = rooms;
        return *this;
    }

    void reset() override {
        windows = 0;
        rooms = 0;
    }

    [[nodiscard]] std::unique_ptr<house> get_result() override {
        auto ptr = std::make_unique<modern_house>(windows, rooms);

        reset();

        return ptr;
    }
};

class director {
  public:
    [[nodiscard]] std::unique_ptr<house>
    build_small_house(builder& builder) const {
        return builder.set_rooms(2).set_windows(2).get_result();
    }

    [[nodiscard]] std::unique_ptr<house>
    build_large_house(builder& builder) const {
        return builder.set_rooms(4).set_windows(4).get_result();
    }
};

int main() {
    medieval_builder first_builder{};
    auto first_house = first_builder.set_windows(3).set_rooms(2).get_result();
    first_house->describe();

    modern_builder second_builder{};
    auto second_house = second_builder.set_windows(4).set_rooms(4).get_result();
    second_house->describe();

    director{}.build_small_house(first_builder)->describe();
    director{}.build_small_house(second_builder)->describe();
    director{}.build_large_house(first_builder)->describe();
    director{}.build_large_house(second_builder)->describe();
}
