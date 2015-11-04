#include <functional>
#include <iostream>

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


int func()
{
    std::cout << "func: (noarg)\n";
    return 5;
}

int func2(int i)
{
    std::cout << "func2: i:" << i << '\n';
    return i;
}

int func3(int i, int j)
{
    std::cout << "func3: i:" << i << '\n';
    std::cout << "func3: j:" << j << '\n';
    return i + j;
}

int main(int argc, char **argv)
{
    Delay<int> D1(1);

    int j{D1};

    Delay<int> D2(func);

    int k = D2;

    Delay<int> D3 = func;

    Delay<int> D4 = func2(3);

    Delay<int> D5([]() {return func2(7);});

    Delay<int> D6(func2, 8);

    Delay<int> D7(func3, 8, 1);

    std::cout << "Yes\n";

    std::cout << "j:" << j << '\n';
    std::cout << "k:" << k << '\n';
    std::cout << "D2:" << D2.get() << '\n';
    std::cout << "D3:" << D3.get() << '\n';
    std::cout << "D4:" << D4.get() << '\n';
    std::cout << "d5:" << D5.get() << '\n';
    std::cout << "d6:" << D6.get() << '\n';
    std::cout << "d7:" << D7.get() << '\n';

    std::cout << "EVAL\n";

    std::cout << "j:" << j << '\n';
    std::cout << "k:" << k << '\n';
    std::cout << "D2:" << D2.get() << '\n';
    std::cout << "D3:" << D3.get() << '\n';
    std::cout << "D4:" << D4.get() << '\n';
    std::cout << "d5:" << D5.get() << '\n';
    std::cout << "d6:" << D6.get() << '\n';
    std::cout << "d7:" << D7.get() << '\n';

 }
