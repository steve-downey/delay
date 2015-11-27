// stream.h                                                           -*-C++-*-
#ifndef INCLUDED_STREAM
#define INCLUDED_STREAM

#include <delay.h>
#include <experimental/optional>
#include <memory>

template <typename Value>
class ConsStream;

template <typename Value>
class ConsCell {
  Value head_;
  ConsStream<Value> tail_;

public:
  ConsCell(Value const& v, ConsStream<Value> const& stream)
      : head_(v), tail_(stream) {
  }

  ConsCell(Value const& v) : head_(v), tail_() {
  }

  Value head() const {
    return head_;
  }

  ConsStream<Value> tail() const {
    return tail_;
  }
};

template <typename Value>
class ConsStream {
  std::shared_ptr<Delay<ConsCell<Value>>> delayed_cell_;

public:
  ConsStream() = default;

  ConsStream(Value value)
      : delayed_cell_(std::make_shared<Delay<ConsCell<Value>>>(
            [value]() { return ConsCell<Value>(value); })) {
  }

  template<typename Func>
  ConsStream(Func&& f)
      : delayed_cell_(std::make_shared<Delay<ConsCell<Value>>>(f)) {
  }

  bool isEmpty() const {
    return !delayed_cell_;
  }

  Value head() const {
    return force(*delayed_cell_).head();
  }

  ConsStream<Value> tail() const {
    return force(*delayed_cell_).tail();
  }
};

#endif
