
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
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

    // 用 std::function 承载回调: 裸函数指针无法接收捕获 lambda / 成员函数
    // context 声明为 const void*, 调用方可以直接传字符串字面量, 不必 const_cast
    template<typename Event>
    using Callback = std::function<void(const Event&, const void*)>;

private:
    template<typename Event>
    struct SubscriberList
    {
        struct Entry
        {
            Callback<Event> callback{};
            const void* context = nullptr;
        };

        std::array<Entry, MaxSubscribers> entries{};
        std::size_t count = 0;

        bool add(Callback<Event> callback, const void* context)
        {
            if (!callback)
            {
                return false;
            }

            // 同一「函数指针 + 上下文」不重复注册(捕获 lambda 无法比较, 只做去重尽力而为)
            const auto* target =
                callback.template target<void (*)(const Event&, const void*)>();

            for (std::size_t i = 0; i < count; ++i)
            {
                if (entries[i].context != context)
                {
                    continue;
                }

                const auto* existing =
                    entries[i].callback.template target<void (*)(const Event&, const void*)>();

                if (target != nullptr && existing != nullptr && *target == *existing)
                {
                    return false;
                }
            }

            if (count >= MaxSubscribers)
            {
                return false;
            }

            entries[count++] = {std::move(callback), context};
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
        const void* context = nullptr)
    {
        static_assert(
            (std::is_same_v<Event, Events> || ...),
            "Unsupported event type");

        return std::get<SubscriberList<Event>>(subscribers_)
            .add(std::move(callback), context);
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
    const void* context)
{
    const char* name =
        static_cast<const char*>(context);

    std::cout << "[Temperature][" << name << "] "
              << event.temperature << " C\n";
}

void on_motor(
    const MotorEvent& event,
    const void*)
{
    std::cout << "[Motor] ID="
              << static_cast<int>(event.motor_id)
              << ", angle="
              << event.angle << '\n';
}

void on_position(
    const PositionEvent& event,
    const void*)
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
        "Main");

    bus.subscribe<TemperatureEvent>(
        on_temperature,
        "Logger");

    bus.subscribe<MotorEvent>(on_motor);
    bus.subscribe<PositionEvent>(on_position);

    // 捕获 lambda 也能订阅(裸函数指针做不到)
    int lambda_hits = 0;
    bus.subscribe<MotorEvent>(
        [&lambda_hits](const MotorEvent& event, const void*)
        {
            ++lambda_hits;
            std::cout << "[Lambda][Motor] angle="
                      << event.angle << '\n';
        });

    // 重复注册同一「函数 + 上下文」会被拒绝
    const bool duplicated =
        bus.subscribe<TemperatureEvent>(on_temperature, "Main");
    std::cout << "duplicate subscribe accepted: "
              << duplicated << '\n';

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

    std::cout << "lambda hits: " << lambda_hits << '\n';

    return 0;
}
