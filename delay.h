// delay.h                                                            -*-C++-*-
#ifndef INCLUDED_DELAY
#define INCLUDED_DELAY


#include <functional>

template <typename T>
class Delay
{

    T value_;
    std::function<T()> func_;
    bool evaled_;

  public:
    Delay(T&& t) : value_(t), evaled_(true)
    {}

    template<typename Action>
    Delay(Action&& A) : func_(std::forward<Action>(A)), evaled_(false)
    {}

    template<typename F, typename... Args>
    Delay(F&& f, Args&&... args)
    : func_([&]() {
            return f(std::forward<Args>(args)...);
        }),
      evaled_(false)
    {
    }

    T const& get()
    {
        if (!evaled_)
            value_ = func_();

        evaled_ = true;

        return value_;
    }

    operator T()
    {
        return get();
    }
};

#endif
