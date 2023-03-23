#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

template <typename Self>
class prototype {
  public:
    virtual ~prototype() = default;

    [[nodiscard]] virtual std::unique_ptr<Self> copy() const = 0;
};

class color : public prototype<color> {
  public:
    virtual void render() const = 0;
};

class rgb_color : public color {
    std::uint8_t red{};
    std::uint8_t green{};
    std::uint8_t blue{};

  public:
    rgb_color(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
        : red{red}, green{green}, blue{blue} {}

    void render() const override {
        std::cout << "RGB(" << +red << ", " << +green << ", " << +blue << ")"
                  << std::endl;
    }

    [[nodiscard]] std::unique_ptr<color> copy() const override {
        return std::make_unique<rgb_color>(red, green, blue);
    }
};

class hsl_color : public color {
    float hue{};
    float saturation{};
    float lightness{};

  public:
    hsl_color(float hue, float saturation, float lightness)
        : hue{hue}, saturation{saturation}, lightness{lightness} {}

    void render() const override {
        std::cout << "HSL(" << hue << ", " << saturation << ", " << lightness
                  << ")" << std::endl;
    }

    [[nodiscard]] std::unique_ptr<color> copy() const override {
        return std::make_unique<hsl_color>(hue, saturation, lightness);
    }
};

class color_factory {
    std::unordered_map<std::string, std::unique_ptr<color>> colors;

  public:
    void add(std::string name, std::unique_ptr<color> color) {
        colors.emplace(std::move(name), std::move(color));
    }

    std::unique_ptr<color> create(const std::string& name) {
        auto it = colors.find(name);
        if (it == colors.end()) {
            throw std::invalid_argument("unknown color");
        }
        return it->second->copy();
    }
};

int main() {
    color_factory factory;

    factory.add("red", std::make_unique<rgb_color>(255, 0, 0));
    factory.add("green", std::make_unique<rgb_color>(0, 255, 0));
    factory.add("blue", std::make_unique<rgb_color>(0, 0, 255));

    factory.add("yellow", std::make_unique<hsl_color>(60, 1, 0.5));

    factory.create("red")->render();
    factory.create("yellow")->render();

    auto color = factory.create("blue");
    auto copy = color->copy();
    color->render();
    std::cout << color.get() << std::endl;
    copy->render();
    std::cout << copy.get() << std::endl;
}
