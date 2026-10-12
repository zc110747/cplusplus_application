// ---------------------------------------------------------------------------
// cppcoroutine/result.hpp
//
// Error taxonomy + a tiny `Result<T>` used by the non-throwing ("try_")
// variants of the channel API.  Throwing paths use the exception types
// declared here.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_RESULT_HPP
#define CPPCOROUTINE_RESULT_HPP

#include <exception>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "cppcoroutine/config.hpp"

namespace cppcoroutine {

/// Coarse error classification.  Kept deliberately small; `what()` carries the
/// human-readable detail.
enum class ErrorCode {
  None = 0,
  Cancelled,
  ChannelClosed,
  ObjectDestroyed,
  WouldBlock,
  InvalidTask,
  Timeout,
};

/// Base class for every exception the library itself throws.
class CoroutineError : public std::runtime_error {
 public:
  CoroutineError(ErrorCode code, std::string message)
      : std::runtime_error(std::move(message)), code_(code) {}

  [[nodiscard]] ErrorCode code() const noexcept { return code_; }

 private:
  ErrorCode code_;
};

/// Thrown from a cancellation checkpoint when the owning task has been
/// cancelled (explicitly, or because the scheduler is shutting down).
class CancellationError : public CoroutineError {
 public:
  explicit CancellationError(TaskId id)
      : CoroutineError(ErrorCode::Cancelled,
                       "task " + std::to_string(id) + " was cancelled"),
        task_(id) {}

  [[nodiscard]] TaskId task_id() const noexcept { return task_; }

 private:
  TaskId task_;
};

/// Thrown when a channel operation cannot complete because the channel was
/// closed (and, for `receive`, its buffer is drained).
class ChannelClosedError : public CoroutineError {
 public:
  explicit ChannelClosedError(std::string message)
      : CoroutineError(ErrorCode::ChannelClosed, std::move(message)) {}
};

/// Thrown when the object a coroutine was waiting on (Event, Channel) is
/// destroyed while waiters are still parked inside it.  Guarantees that no
/// coroutine ever dereferences a destroyed wait object.
class ObjectDestroyedError : public CoroutineError {
 public:
  explicit ObjectDestroyedError(std::string message)
      : CoroutineError(ErrorCode::ObjectDestroyed, std::move(message)) {}
};

/// Returned by `try_send` / `try_receive` when the operation would have to
/// block.
class WouldBlockError : public CoroutineError {
 public:
  WouldBlockError() : CoroutineError(ErrorCode::WouldBlock, "operation would block") {}
};

/// `Result<T>` either holds a value or an `std::exception_ptr`.
/// It is deliberately minimal - it is a transport for "value or error",
/// not a general purpose monad.
template <class T>
class Result {
 public:
  using value_type = T;

  Result(T value)  // NOLINT(google-explicit-constructor): intentionally implicit
      : value_(std::move(value)) {}

  Result(std::exception_ptr error)  // NOLINT(google-explicit-constructor)
      : error_(std::move(error)) {}

  [[nodiscard]] bool has_value() const noexcept { return value_.has_value(); }
  explicit operator bool() const noexcept { return has_value(); }

  /// Precondition: `has_value()`.
  T& value() & {
    if (!value_) {
      std::rethrow_exception(error_);
    }
    return *value_;
  }

  /// Precondition: `has_value()`.
  const T& value() const& {
    if (!value_) {
      std::rethrow_exception(error_);
    }
    return *value_;
  }

  /// Precondition: `has_value()`.
  T&& value() && {
    if (!value_) {
      std::rethrow_exception(error_);
    }
    return std::move(*value_);
  }

  T value_or(T fallback) const {
    return value_ ? *value_ : std::move(fallback);
  }

  [[nodiscard]] std::exception_ptr error() const noexcept { return error_; }

 private:
  std::optional<T> value_{};
  std::exception_ptr error_{};
};

/// `void` specialisation - only carries an error (or nothing).
template <>
class Result<void> {
 public:
  using value_type = void;

  Result() noexcept = default;
  Result(std::exception_ptr error)  // NOLINT(google-explicit-constructor)
      : error_(std::move(error)) {}

  /// Explicit construction of a successful empty result.
  static Result success() noexcept { return Result{}; }

  [[nodiscard]] bool has_value() const noexcept { return !error_; }
  explicit operator bool() const noexcept { return has_value(); }

  void value() const {
    if (error_) {
      std::rethrow_exception(error_);
    }
  }

  [[nodiscard]] std::exception_ptr error() const noexcept { return error_; }

 private:
  std::exception_ptr error_{};
};

}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_RESULT_HPP
