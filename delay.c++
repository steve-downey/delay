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

ConsStream<std::tuple<int, int, int>> triples() {
  return bind2(iota(1), [](int z) {
      return bind2(rangeFrom(1, z), [z](int x) {
          return bind2(rangeFrom(x, z), [x, z](int y) {
              return then2(guard(x*x + y*y == z*z), [x, y, z]() {
                  return make_consstream(std::make_tuple(x, y, z));
                });
            });
        });
    });
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

    Delay<int> D6 = delay(func2, 8);

    Delay<int> D7 = delay(func3, 8, 1);

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
    ConsStream<int> cs2 = make_stream(1);
    ConsStream<int> cs2a(1);

    std::cout << "cs1:" << cs1.isEmpty() << '\n';
    std::cout << "cs2:" << cs2.isEmpty() << '\n';
    std::cout << "cs2a:" << cs2a.isEmpty() << '\n';

    ConsStream<int> cs3(cs2);
    std::cout << "cs3:" << cs3.isEmpty() << '\n';


    // auto trip = triples();

    // auto tenTrips = take(trip, 300);
    auto lastOne = last(take(triples(), 10));
    int x, y, z;
    std::tie(x,y,z) = lastOne;
    std::cout << x << ',' << y << ',' << z << '\n';
 }
