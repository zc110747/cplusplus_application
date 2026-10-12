// ---------------------------------------------------------------------------
// cppcoroutine/block_channel.hpp
//
// Byte-block channel: the primitive for moving network frames, DMA buffers or
// acquisition blocks between coroutines without copying them.
//
// `DataBlock` owns its payload and is moved, never copied, by the channel.
// Ownership rules (see docs/lifecycle.md):
//   * send succeeded   -> the channel owns the block (buffered) or the
//                         receiver owns it (rendezvous).
//   * send cancelled /   -> the block stays owned by the sender and is
//     send failed          destroyed together with its frame.
//   * receive succeeded -> the receiver owns the block.
//   * channel closed    -> buffered blocks are still delivered (drain);
//                          parked senders fail and keep their blocks.
//
// Header-only: the BlockChannel definitions below used to live in
// src/channel.cpp.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_BLOCK_CHANNEL_HPP
#define CPPCOROUTINE_BLOCK_CHANNEL_HPP

#include <atomic>
#include <coroutine>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

#include "cppcoroutine/channel.hpp"
#include "cppcoroutine/config.hpp"
#include "cppcoroutine/result.hpp"
#include "cppcoroutine/scheduler.hpp"

namespace cppcoroutine {

/// A dynamically sized, self-describing block of bytes.
struct DataBlock {
  std::vector<std::byte> data{};
  /// Channel-issued ordering stamp (0 == unstamped).
  std::uint64_t sequence = 0;
  /// Free-form producer id (device index, socket id, ...).
  std::uint64_t source = 0;
  /// Time at which the producer produced the block.
  Scheduler::clock::time_point timestamp{};

  [[nodiscard]] std::size_t size() const noexcept { return data.size(); }
  [[nodiscard]] bool empty() const noexcept { return data.empty(); }

  friend bool operator==(const DataBlock& lhs, const DataBlock& rhs) noexcept {
    return lhs.sequence == rhs.sequence && lhs.source == rhs.source && lhs.data == rhs.data;
  }
  friend bool operator!=(const DataBlock& lhs, const DataBlock& rhs) noexcept {
    return !(lhs == rhs);
  }
};

/// Diagnostics for a block channel.
struct BlockChannelStats {
  std::uint64_t blocks_sent = 0;
  std::uint64_t blocks_received = 0;
  std::uint64_t bytes_sent = 0;
  std::uint64_t bytes_received = 0;
  /// Number of times a received block did not continue the issued sequence.
  std::uint64_t sequence_gaps = 0;
};

class BlockChannel {
 public:
  explicit BlockChannel(Scheduler& scheduler,
                        std::size_t capacity = default_block_channel_capacity,
                        std::size_t byte_budget = default_block_channel_byte_budget);

  ~BlockChannel() = default;

  BlockChannel(const BlockChannel&) = delete;
  BlockChannel& operator=(const BlockChannel&) = delete;
  BlockChannel(BlockChannel&&) = delete;
  BlockChannel& operator=(BlockChannel&&) = delete;

  /// Issues the next channel-wide ordering stamp.  Producers should call this
  /// instead of inventing their own sequence numbers.
  [[nodiscard]] std::uint64_t next_sequence() noexcept {
    return next_sequence_.fetch_add(1, std::memory_order_relaxed);
  }

  // -------------------------------------------------------------------------
  // Coroutine API (adds statistics on top of Channel<DataBlock>)
  // -------------------------------------------------------------------------

  struct SendAwaiter {
    BlockChannel* owner = nullptr;
    std::size_t bytes = 0;
    detail::ChannelSendAwaiter<DataBlock> inner;

    SendAwaiter(BlockChannel* owner_in, DataBlock block)
        : owner(owner_in), bytes(block.size()), inner(&owner_in->channel_, std::move(block)) {}

    SendAwaiter(const SendAwaiter&) = delete;
    SendAwaiter& operator=(const SendAwaiter&) = delete;
    SendAwaiter(SendAwaiter&&) = delete;
    SendAwaiter& operator=(SendAwaiter&&) = delete;

    [[nodiscard]] bool await_ready() const noexcept { return false; }
    bool await_suspend(std::coroutine_handle<> handle) { return inner.await_suspend(handle); }
    void await_resume() {
      inner.await_resume();
      owner->record_sent(bytes);
    }
  };

  struct ReceiveAwaiter {
    BlockChannel* owner = nullptr;
    detail::ChannelReceiveAwaiter<DataBlock> inner;

    explicit ReceiveAwaiter(BlockChannel* owner_in) : owner(owner_in), inner(&owner_in->channel_) {}

    ReceiveAwaiter(const ReceiveAwaiter&) = delete;
    ReceiveAwaiter& operator=(const ReceiveAwaiter&) = delete;
    ReceiveAwaiter(ReceiveAwaiter&&) = delete;
    ReceiveAwaiter& operator=(ReceiveAwaiter&&) = delete;

    [[nodiscard]] bool await_ready() const noexcept { return false; }
    bool await_suspend(std::coroutine_handle<> handle) { return inner.await_suspend(handle); }
    DataBlock await_resume() {
      DataBlock block = inner.await_resume();
      owner->record_received(block);
      return block;
    }
  };

  [[nodiscard]] SendAwaiter send(DataBlock block) { return SendAwaiter{this, std::move(block)}; }
  [[nodiscard]] ReceiveAwaiter receive() { return ReceiveAwaiter{this}; }

  Result<void> try_send(DataBlock block);
  Result<DataBlock> try_receive();

  // -------------------------------------------------------------------------
  // State
  // -------------------------------------------------------------------------

  void close() { channel_.close(); }
  [[nodiscard]] bool closed() const noexcept { return channel_.closed(); }
  [[nodiscard]] std::size_t size() const noexcept { return channel_.size(); }
  [[nodiscard]] std::size_t capacity() const noexcept { return channel_.capacity(); }
  [[nodiscard]] std::size_t buffered_bytes() const noexcept { return channel_.buffered_bytes(); }
  [[nodiscard]] std::size_t sender_count() const { return channel_.sender_count(); }
  [[nodiscard]] std::size_t receiver_count() const { return channel_.receiver_count(); }

  [[nodiscard]] BlockChannelStats stats() const noexcept { return stats_; }

  /// Enables/disables the ordering check (enabled by default).
  void enable_sequence_check(bool enabled) noexcept { sequence_check_ = enabled; }

  /// The next sequence number the consumer expects; 1 before the first block.
  [[nodiscard]] std::uint64_t expected_sequence() const noexcept { return expected_sequence_; }

 private:
  static std::size_t measure_block(const DataBlock& block) noexcept { return block.data.size(); }

  void record_sent(std::size_t bytes) noexcept {
    ++stats_.blocks_sent;
    stats_.bytes_sent += bytes;
  }

  void record_received(const DataBlock& block) noexcept {
    ++stats_.blocks_received;
    stats_.bytes_received += block.size();
    if (sequence_check_ && block.sequence != 0) {
      if (block.sequence != expected_sequence_) {
        ++stats_.sequence_gaps;
      }
      expected_sequence_ = block.sequence + 1;
    }
  }

  Channel<DataBlock> channel_;
  std::atomic<std::uint64_t> next_sequence_{1};
  std::uint64_t expected_sequence_ = 1;
  bool sequence_check_ = true;
  BlockChannelStats stats_{};
};

// ---------------------------------------------------------------------------
// BlockChannel implementation (was src/channel.cpp)
// ---------------------------------------------------------------------------

inline BlockChannel::BlockChannel(Scheduler& scheduler, std::size_t capacity,
                                  std::size_t byte_budget)
    : channel_(scheduler, capacity, byte_budget, &BlockChannel::measure_block) {}

inline Result<void> BlockChannel::try_send(DataBlock block) {
  const std::size_t bytes = block.size();
  Result<void> result = channel_.try_send(std::move(block));
  if (result.has_value()) {
    record_sent(bytes);
  }
  return result;
}

inline Result<DataBlock> BlockChannel::try_receive() {
  Result<DataBlock> result = channel_.try_receive();
  if (result.has_value()) {
    record_received(result.value());
  }
  return result;
}

}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_BLOCK_CHANNEL_HPP
