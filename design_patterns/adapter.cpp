#include <iostream>

namespace library1 {
    class connection {
      public:
        [[nodiscard]] int query() const { return 1; }
    };
} // namespace library1

namespace library2 {
    class connection {
      public:
        [[nodiscard]] int select() const { return 2; }
    };
} // namespace library2

class connection_interface {
  public:
    virtual ~connection_interface() = default;

    [[nodiscard]] virtual int get_data() const = 0;
};

class library1_connection : public connection_interface {
    library1::connection connection;

  public:
    // Move not required, but imagining a more complex library in the
    // background.
    explicit library1_connection(library1::connection connection)
        : connection{std::move(connection)} {}

    [[nodiscard]] int get_data() const final { return connection.query(); }
};

class library2_connection : public connection_interface {
    library2::connection connection;

  public:
    // Move not required, but imagining a more complex library in the
    // background.
    explicit library2_connection(library2::connection connection)
        : connection{std::move(connection)} {}

    [[nodiscard]] int get_data() const final { return connection.select(); }
};

void get_data(const connection_interface& connection) {
    std::cout << connection.get_data() << std::endl;
}

int main() {
    get_data(library1_connection{library1::connection{}});
    get_data(library2_connection{library2::connection{}});
}
