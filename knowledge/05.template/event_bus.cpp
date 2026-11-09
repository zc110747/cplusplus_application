
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

// ============================================================
// 1. Event definitions
// ============================================================

struct TemperatureEvent
{
    float temperature;
};

struct MotorEvent
{
    std::uint8_t motor_id;
    float angle;
};

struct PositionEvent
{
    float x;
    float y;
    float z;
};

// ============================================================
// 2. Event priority
// ============================================================

enum class Priority : std::uint8_t
{
    Low    = 0,
    Normal = 1,
    High   = 2,
    Urgent = 3
};

// ============================================================
// 3. Compile-time event index
// ============================================================

template<typename T, typename... Types>
struct TypeIndex;

template<typename T, typename... Rest>
struct TypeIndex<T, T, Rest...>
    : std::integral_constant<std::size_t, 0>
{
};

template<typename T, typename First, typename... Rest>
struct TypeIndex<T, First, Rest...>
    : std::integral_constant<
          std::size_t,
          1 + TypeIndex<T, Rest...>::value>
{
};

template<typename T, typename... Types>
inline constexpr std::size_t type_index_v =
    TypeIndex<T, Types...>::value;

// ============================================================
// 4. EventBus
// ============================================================

template<
    std::size_t QueueCapacity,
    std::size_t MaxSubscribers,
    typename... Events>
class StaticEventBus
{
    static_assert(sizeof...(Events) > 0);
    static_assert(QueueCapacity > 0);
    static_assert(MaxSubscribers > 0);

public:
    static constexpr std::size_t event_count = sizeof...(Events);

    using EventVariant = std::variant<Events...>;

    // --------------------------------------------------------
    // Event ID
    // --------------------------------------------------------

    template<typename Event>
    static constexpr std::size_t event_id()
    {
        static_assert(
            (std::is_same_v<Event, Events> || ...),
            "Event type is not registered in this EventBus");

        return type_index_v<Event, Events...>;
    }

    // --------------------------------------------------------
    // Callback interface
    // --------------------------------------------------------

    template<typename Event>
    using Callback = void (*)(const Event&, void*);

private:
    template<typename Event>
    struct SubscriberList
    {
        struct Entry
        {
            Callback<Event> callback = nullptr;
            void* context = nullptr;
        };

        std::array<Entry, MaxSubscribers> entries{};
        std::size_t count = 0;

        bool add(Callback<Event> callback, void* context)
        {
            if (count >= MaxSubscribers)
            {
                return false;
            }

            entries[count++] = {callback, context};
            return true;
        }

        void dispatch(const Event& event)
        {
            for (std::size_t i = 0; i < count; ++i)
            {
                entries[i].callback(
                    event,
                    entries[i].context);
            }
        }
    };

    // 每种事件拥有独立的订阅者列表
    std::tuple<SubscriberList<Events>...> subscribers_;

    // --------------------------------------------------------
    // Queue item
    // --------------------------------------------------------

    struct QueueItem
    {
        EventVariant event{};
        Priority priority = Priority::Normal;
        std::uint32_t sequence = 0;
    };

    std::array<QueueItem, QueueCapacity> queue_{};

    std::size_t queue_size_ = 0;
    std::uint32_t sequence_ = 0;

    // --------------------------------------------------------
    // Dispatch event
    // --------------------------------------------------------

    template<typename Event>
    void dispatch_one(const Event& event)
    {
        auto& list = std::get<SubscriberList<Event>>(subscribers_);
        list.dispatch(event);
    }

public:
    // --------------------------------------------------------
    // Subscribe
    // --------------------------------------------------------

    template<typename Event>
    bool subscribe(
        Callback<Event> callback,
        void* context = nullptr)
    {
        static_assert(
            (std::is_same_v<Event, Events> || ...),
            "Unsupported event type");

        if (callback == nullptr)
        {
            return false;
        }

        return std::get<SubscriberList<Event>>(subscribers_)
            .add(callback, context);
    }

    // --------------------------------------------------------
    // Publish event
    // --------------------------------------------------------

    template<typename Event>
    bool publish(
        const Event& event,
        Priority priority = Priority::Normal)
    {
        static_assert(
            (std::is_same_v<Event, Events> || ...),
            "Unsupported event type");

        if (queue_size_ >= QueueCapacity)
        {
            return false;
        }

        queue_[queue_size_++] = QueueItem{
            EventVariant{event},
            priority,
            sequence_++
        };

        return true;
    }

    // --------------------------------------------------------
    // Check queue
    // --------------------------------------------------------

    bool empty() const
    {
        return queue_size_ == 0;
    }

    std::size_t size() const
    {
        return queue_size_;
    }

    // --------------------------------------------------------
    // Pop highest priority event
    // --------------------------------------------------------

    bool pop(EventVariant& output)
    {
        if (queue_size_ == 0)
        {
            return false;
        }

        std::size_t best = 0;

        for (std::size_t i = 1; i < queue_size_; ++i)
        {
            const auto current_priority =
                static_cast<std::uint8_t>(queue_[i].priority);

            const auto best_priority =
                static_cast<std::uint8_t>(queue_[best].priority);

            if (current_priority > best_priority)
            {
                best = i;
            }
            else if (
                current_priority == best_priority &&
                queue_[i].sequence < queue_[best].sequence)
            {
                // 相同优先级时 FIFO
                best = i;
            }
        }

        output = queue_[best].event;

        // 移除元素，保持数组连续
        for (std::size_t i = best; i + 1 < queue_size_; ++i)
        {
            queue_[i] = queue_[i + 1];
        }

        --queue_size_;

        return true;
    }

    // --------------------------------------------------------
    // Consumer: process one event
    // --------------------------------------------------------

    bool process_one()
    {
        EventVariant event;

        if (!pop(event))
        {
            return false;
        }

        std::visit(
            [this](const auto& value)
            {
                dispatch_one(value);
            },
            event);

        return true;
    }

    // --------------------------------------------------------
    // Consumer: process all queued events
    // --------------------------------------------------------

    void process_all()
    {
        while (process_one())
        {
        }
    }
};

// ============================================================
// 5. Callback functions
// ============================================================

void on_temperature(
    const TemperatureEvent& event,
    void* context)
{
    const char* name =
        static_cast<const char*>(context);

    std::cout << "[Temperature][" << name << "] "
              << event.temperature << " C\n";
}

void on_motor(
    const MotorEvent& event,
    void*)
{
    std::cout << "[Motor] ID="
              << static_cast<int>(event.motor_id)
              << ", angle="
              << event.angle << '\n';
}

void on_position(
    const PositionEvent& event,
    void*)
{
    std::cout << "[Position] "
              << event.x << ", "
              << event.y << ", "
              << event.z << '\n';
}

// ============================================================
// 6. event_bus_manage
// ============================================================
int event_bus_manage(void)
{
    using Bus = StaticEventBus<
        16,  // Queue capacity
        4,   // Max subscribers per event
        TemperatureEvent,
        MotorEvent,
        PositionEvent
    >;

    Bus bus;

    // Compile-time Event ID
    static_assert(Bus::event_id<TemperatureEvent>() == 0);
    static_assert(Bus::event_id<MotorEvent>() == 1);
    static_assert(Bus::event_id<PositionEvent>() == 2);

    std::cout << "TemperatureEvent ID = "
              << Bus::event_id<TemperatureEvent>()
              << '\n';

    std::cout << "MotorEvent ID = "
              << Bus::event_id<MotorEvent>()
              << '\n';

    std::cout << "PositionEvent ID = "
              << Bus::event_id<PositionEvent>()
              << '\n';

    // Subscribe
    bus.subscribe<TemperatureEvent>(
        on_temperature,
        const_cast<char*>("Main"));

    bus.subscribe<TemperatureEvent>(
        on_temperature,
        const_cast<char*>("Logger"));

    bus.subscribe<MotorEvent>(on_motor);
    bus.subscribe<PositionEvent>(on_position);

    // Producer: publish events
    bus.publish(
        TemperatureEvent{36.5f},
        Priority::Low);

    bus.publish(
        MotorEvent{1, 90.0f},
        Priority::Normal);

    bus.publish(
        PositionEvent{10.0f, 20.0f, 30.0f},
        Priority::High);

    bus.publish(
        MotorEvent{2, 180.0f},
        Priority::Urgent);

    std::cout << "\nQueued events: "
              << bus.size() << '\n';

    // Consumer: process queued events
    std::cout << "\n========== Consumer ==========\n";

    bus.process_all();

    return 0;
}
