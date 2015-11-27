#include <functional>
#include <iostream>

#include <delay.h>
#include <stream.h>

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


    ConsCell<int> cc2(1);

    ConsStream<int> cs1;
    ConsStream<int> cs2(1);

    std::cout << "cs1:" << cs1.isEmpty() << '\n';
    std::cout << "cs2:" << cs2.isEmpty() << '\n';
 }
