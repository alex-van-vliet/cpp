#include <iostream>
#include <memory>

class plus_node;

class integer_node;

class visitor {
  public:
    virtual ~visitor() = default;

    virtual void visit(const plus_node& node) = 0;

    virtual void visit(const integer_node& node) = 0;
};

class element {
  public:
    virtual ~element() = default;

    virtual void accept(visitor& visitor) const = 0;
};

class plus_node : public element {
    std::unique_ptr<element> left_operand;
    std::unique_ptr<element> right_operand;

  public:
    plus_node(std::unique_ptr<element> left_operand,
              std::unique_ptr<element> right_operand)
        : left_operand{std::move(left_operand)}, right_operand{std::move(
                                                     right_operand)} {}

    void accept(visitor& visitor) const override { visitor.visit(*this); }

    [[nodiscard]] const element& get_left_operand() const {
        return *left_operand;
    }

    [[nodiscard]] const element& get_right_operand() const {
        return *right_operand;
    }
};

class integer_node : public element {
    int value;

  public:
    explicit integer_node(int value) : value{value} {}

    void accept(visitor& visitor) const override { visitor.visit(*this); }

    [[nodiscard]] int get_value() const { return value; }
};

class pretty_printer : public visitor {
  public:
    void visit(const plus_node& node) override {
        std::cout << "(";
        node.get_left_operand().accept(*this);
        std::cout << ") + (";
        node.get_right_operand().accept(*this);
        std::cout << ")";
    }

    void visit(const integer_node& node) override {
        std::cout << node.get_value();
    }
};

class calculator : public visitor {
    int result{0};

  public:
    void visit(const plus_node& node) override {
        result = 0;
        node.get_left_operand().accept(*this);
        int left = result;

        result = 0;
        node.get_right_operand().accept(*this);
        int right = result;

        result = left + right;
    }

    void visit(const integer_node& node) override { result = node.get_value(); }

    [[nodiscard]] int get_result() const { return result; }
};

int main() {
    auto tree = std::make_unique<plus_node>(
        std::make_unique<plus_node>(std::make_unique<integer_node>(3),
                                    std::make_unique<integer_node>(-2)),
        std::make_unique<integer_node>(4));

    pretty_printer printer{};
    tree->accept(printer);
    std::cout << std::endl;

    calculator calculator;
    tree->accept(calculator);
    std::cout << calculator.get_result() << std::endl;
}
