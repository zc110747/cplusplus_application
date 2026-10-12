// ---------------------------------------------------------------------------
// cppcoroutine/config.hpp
//
// Compile-time configuration, version macros and fundamental type aliases.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_CONFIG_HPP
#define CPPCOROUTINE_CONFIG_HPP

#include <cstddef>
#include <cstdint>

#define CPPCOROUTINE_VERSION_MAJOR 1
#define CPPCOROUTINE_VERSION_MINOR 0
#define CPPCOROUTINE_VERSION_PATCH 0
#define CPPCOROUTINE_VERSION_STRING "1.0.0"

namespace cppcoroutine {

/// Stable identifier for a task registered with a Scheduler.
/// `invalid_task_id` is never handed out by the runtime.
using TaskId = std::uint64_t;

/// Sentinel value meaning "no task".
inline constexpr TaskId invalid_task_id = 0;

/// Default capacity of a bounded channel when the user does not specify one.
inline constexpr std::size_t default_channel_capacity = 64;

/// Default capacity of a block (byte) channel.
inline constexpr std::size_t default_block_channel_capacity = 16;

/// Soft upper bound on the number of bytes a single block channel may retain.
inline constexpr std::size_t default_block_channel_byte_budget = 4u * 1024u * 1024u;

}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_CONFIG_HPP
