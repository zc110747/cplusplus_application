// ---------------------------------------------------------------------------
// cppcoroutine/value_channel.hpp
//
// `ValueChannel<T>` is the name this project uses for the single-value channel
// described in section 7.2 of the specification.  It is exactly
// `Channel<T>` - the alias exists so that the two communication primitives
// ("value channel" and "block channel") can be named symmetrically, and so
// that user code reads well:
//
//     cppcoroutine::ValueChannel<int> channel(scheduler);
//     cppcoroutine::BlockChannel      blocks(scheduler);
//
// See cppcoroutine/channel.hpp for the full semantics (bounded FIFO, blocking
// send/receive, close/drain, cancellation).
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_VALUE_CHANNEL_HPP
#define CPPCOROUTINE_VALUE_CHANNEL_HPP

#include "cppcoroutine/channel.hpp"

namespace cppcoroutine {

template <class T>
using ValueChannel = Channel<T>;

}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_VALUE_CHANNEL_HPP
