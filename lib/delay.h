// delay.h                                                            -*-C++-*-
#ifndef INCLUDED_DELAY
#define INCLUDED_DELAY

#include <functional>
#include <experimental/optional>

#include <iostream>

template <typename Value>
class Delay {
  std::function<Value()> func_;
  mutable std::experimental::optional<Value>  value_;
  mutable bool evaled_;

  using Func = std::function<Value()>;
  template <typename Action>
  using isFuncConv = std::is_convertible<Action, Func>;

public:
  Delay(Value&& value) : value_(value), evaled_(true) {
  }

  template <typename Action,
            typename = typename std::enable_if<isFuncConv<Action>::value>::type>
  Delay(Action&& A) : func_(std::forward<Action>(A)), evaled_(false) {
  }

  Value const& get() const {
    if (!evaled_) {
      value_ = func_();
    }

    evaled_ = true;

    return value_.value();
  }

  operator Value const&() const {
    return get();
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
  return Delay<Value>([args..., f_ = std::forward<F>(f) ]() { return f_(args...); });
}


#endif
