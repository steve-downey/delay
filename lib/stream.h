// stream.h                                                           -*-C++-*-
#ifndef INCLUDED_STREAM
#define INCLUDED_STREAM

#include <delay.h>
#include <experimental/optional>
#include <iterator>
#include <memory>

template <typename Value>
class ConsStream;

template <typename Value>
class ConsStreamIterator;

template <typename Value>
class ConsCell {
  Value head_;
  ConsStream<Value> tail_;

  friend class ConsStreamIterator<Value>;
  friend class ConsStreamIterator<const Value>;

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

  friend class ConsStreamIterator<Value>;
public:
  ConsStream() = default;

  ConsStream(Value value)
      : delayed_cell_(std::make_shared<Delay<ConsCell<Value>>>(
            [value]() { return ConsCell<Value>(value); })) {
  }

  template <typename Func,
            typename = typename std::enable_if<!std::is_convertible<Func, ConsStream>::value>::type>
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

  typedef ConsStreamIterator<Value> iterator;

  iterator begin() {
    return iterator(delayed_cell_);
  };

  iterator end() {
    return iterator();
  }

};

template <typename Value>
ConsStream<Value> make_stream(Value v) {
  return ConsStream<Value>([v]() {return v;});
}

template <typename Value>
class ConsStreamIterator : public std::iterator<std::forward_iterator_tag,
                                                std::remove_cv_t<Value>,
                                                std::ptrdiff_t,
                                                Value*,
                                                Value&> {
  std::shared_ptr<Delay<ConsCell<Value>>> delayed_cell_;

  explicit ConsStreamIterator(std::shared_ptr<Delay<ConsCell<Value>>> cell)
      : delayed_cell_(cell) {
  }

  friend class ConsStream<Value>;
public:
  ConsStreamIterator() = default; // Default construct gives end.

  void swap(ConsStreamIterator& other) noexcept {
    using std::swap;
    swap(delayed_cell_, other.delayed_cell_);
  }

  ConsStreamIterator& operator++() // Pre-increment
  {
    delayed_cell_ = force(*delayed_cell_).tail().delayed_cell_;
    return *this;
  }

  ConsStreamIterator operator++(int) // Post-increment
  {
    ConsStreamIterator tmp(*this);
    delayed_cell_ = force(*delayed_cell_).tail().delayed_cell_;
    return tmp;
  }

  // two-way comparison: v.begin() == v.cbegin() and vice versa
  template <class OtherType>
  bool operator==(const ConsStreamIterator<OtherType>& rhs) const {
    return delayed_cell_ == rhs.delayed_cell_;
  }

  template <class OtherType>
  bool operator!=(const ConsStreamIterator<OtherType>& rhs) const {
    return delayed_cell_ != rhs.delayed_cell_;
  }

  Value const& operator*() const {
    return force(*delayed_cell_).head_;
  }

  Value const* operator->() const {
    return &force(*delayed_cell_).head_;
  }

};

template<typename Value>
ConsStream<Value> rangeFrom(Value n, Value m) {
  if (n > m) {
    return ConsStream<Value>();
  }
  return ConsStream<Value>([n, m]() {
      return ConsCell<Value>(n, rangeFrom(n+1, m));
    });
}

template<typename Value>
ConsStream<Value> iota(Value n = Value()) {
  return ConsStream<Value>([n]() {
      return ConsCell<Value>(n, iota(n+1));
    });
}

template<typename Value>
ConsStream<Value> take(ConsStream<Value> const& strm, int n) {
  if (n == 0 || strm.isEmpty()) {
    return ConsStream<Value>();
  }
  return ConsStream<Value>([strm, n]() {
      return ConsCell<Value>(strm.head(), take(strm.tail(), n-1));
    });
}


#endif
