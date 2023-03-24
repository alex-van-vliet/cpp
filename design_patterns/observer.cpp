#include <algorithm>
#include <iostream>
#include <vector>

template <typename SubscriberInterface, typename EventType>
concept subscriber_interface_concept =
    requires(SubscriberInterface& interface, const EventType& event) {
        interface.update(event);
    };

template <typename SubscriberInterface, typename EventType>
requires subscriber_interface_concept<SubscriberInterface, EventType>
class publisher {
    using reference = std::reference_wrapper<SubscriberInterface>;

    std::vector<reference> subscribers;

  public:
    void subscribe(SubscriberInterface& subscriber) {
        subscribers.emplace_back(std::ref(subscriber));
    }

    void unsubscribe(const SubscriberInterface& subscriber) {
        subscribers.erase(std::remove_if(subscribers.begin(), subscribers.end(),
                                         [&subscriber](auto& el) {
                                             return &el.get() == &subscriber;
                                         }),
                          subscribers.end());
    }

    void notify(const EventType& event) {
        for (auto& subscriber : subscribers)
            subscriber.get().update(event);
    }
};

struct mouse_event {
    int x;
    int y;
    bool pressed;

    friend std::ostream& operator<<(std::ostream& os,
                                    const mouse_event& event) {
        os << "x: " << event.x << " y: " << event.y
           << " pressed: " << event.pressed;
        return os;
    }
};

class mouse_subscriber {
  public:
    virtual ~mouse_subscriber() = default;
    virtual void update(const mouse_event&) = 0;
};

class camera : public mouse_subscriber {
  public:
    void update(const mouse_event& event) final {
        std::cout << "Updating camera with new mouse position: " << event
                  << std::endl;
    }
};

class rendering : public mouse_subscriber {
  public:
    void update(const mouse_event& event) final {
        std::cout << "Detecting entities in frame with mouse position: "
                  << event << std::endl;
    }
};

int main() {
    camera camera;
    rendering rendering;

    publisher<mouse_subscriber, mouse_event> mouse;

    mouse.subscribe(camera);
    mouse.subscribe(rendering);

    mouse.notify(mouse_event{10, 10, false});
    mouse.notify(mouse_event{10, 10, true});

    mouse.unsubscribe(rendering);
    mouse.notify(mouse_event{10, 20, true});

    return 0;
}
