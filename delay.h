// delay.h                                                            -*-C++-*-
#ifndef INCLUDED_DELAY
#define INCLUDED_DELAY


#include <functional>

template <typename Value>
class Delay
{

    Value value_;
    std::function<Value()> func_;
    bool evaled_;

  public:
    Delay(Value&& value) : value_(value), evaled_(true)
    {}

    template<typename Action>
    Delay(Action&& A) : func_(std::forward<Action>(A)), evaled_(false)
    {}

    template<typename F, typename... Args>
    Delay(F&& f, Args&&... args)
    : func_([=, &f]() {
            return f(args...);
        }),
      evaled_(false)
    {
    }

    Value const& get()
    {
        if (!evaled_)
            value_ = func_();

        evaled_ = true;

        return value_;
    }

    operator Value()
    {
        return get();
    }
};

#endif
