// delay.h                                                            -*-C++-*-
#ifndef INCLUDED_DELAY
#define INCLUDED_DELAY

#include <experimental/optional>
#include <functional>

#include <iostream>

template <typename Value>
class Delay {
  std::function<Value()> func_;
  typedef typename std::aligned_storage<sizeof(Value),
                                        std::alignment_of<Value>::value>::type
      Storage;
  mutable Storage value_;
  mutable bool evaled_;

  using Func = std::function<Value()>;
  template <typename Action>
  using isFuncConv = std::is_convertible<Action, Func>;

  template <typename Arg>
  void setValue(Arg&& arg) const {
    ::new (&value_) Value(std::move(arg));
    evaled_ = true;
  }

public:
  Delay(Value const& value) : evaled_(true) {
    ::new (&value_) Value(value);
  }

  Delay(Value&& value) : evaled_(true) {
    ::new (&value_) Value(std::move(value));
  }

  template <typename Action,
            typename = typename std::enable_if<isFuncConv<Action>::value>::type>
  Delay(Action&& A) : func_(std::forward<Action>(A)), evaled_(false) {
  }

  ~Delay() {
    if (evaled_) {
      reinterpret_cast<Value*>(std::addressof(this->value_))->~Value();
    }
  }

  Value const& get() const {
    if (!evaled_) {
      setValue(func_());
    }
    return *reinterpret_cast<Value*>(&value_);
  }

  operator Value const&() const {
    return get();
  }

  bool isForced() {
    return evaled_;
  }
};

template <typename Value>
Value const& force(Delay<Value> const& delay) {
  return delay;
}

template <typename Value>
Value const& force(Delay<Value>&& delay) {
  return std::move(delay);
}

template <typename F, typename... Args>
auto delay(F&& f, Args&&... args) -> Delay<decltype(f(args...))> {
  using Value = decltype(f(args...));
  return Delay<Value>(
      [ args..., f_ = std::forward<F>(f) ]() { return f_(args...); });
}

#endif
