#include <functional>
#include <iostream>

#include <delay.h>
#include <stream.h>

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
    auto lastOne = last(take(triples(), 300));
    int x, y, z;
    std::tie(x,y,z) = lastOne;
    std::cout << x << ',' << y << ',' << z << '\n';
 }
