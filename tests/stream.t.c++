#include <stream.h>

#include "gtest/gtest.h"
using ::testing::Test;

namespace testing {
namespace {}

class StreamTest : public Test {
protected:
  StreamTest() {
  }
  ~StreamTest() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

public:
};

TEST_F(StreamTest, breathingTest) {
    ConsCell<int> cc2(1);

    ConsStream<int> cs1;
    ConsStream<int> cs2 = make_stream(1);
    ConsStream<int> cs3(1);

    EXPECT_TRUE(cs1.isEmpty());
    EXPECT_FALSE(cs2.isEmpty());
    EXPECT_FALSE(cs3.isEmpty());

    int j = -1;
    EXPECT_NO_THROW({
            j = cc2.head();
        });
    EXPECT_EQ(1, j);

    ConsStream<int> str2 = cc2.tail();

    EXPECT_TRUE(str2.isEmpty());

    int k = cs2.head();
    ConsStream<int> str3 = cs2.tail();

    EXPECT_EQ(1, k);
    EXPECT_TRUE(str3.isEmpty());

}

TEST_F(StreamTest, consTest) {
  ConsStream<int> cs(0);
  ConsStream<int> cs1 = cons(1, cs);
  ConsStream<int> stream = cons(2, cs1);

  EXPECT_EQ(2, stream.head());
  EXPECT_EQ(1, stream.tail().head());
  EXPECT_EQ(0, stream.tail().tail().head());
  EXPECT_TRUE(stream.tail().tail().tail().isEmpty());
}

TEST_F(StreamTest, finiteStream) {
  ConsStream<int> stream = rangeFrom(0, 2);
  EXPECT_EQ(0, stream.head());
  EXPECT_EQ(1, stream.tail().head());
  EXPECT_EQ(2, stream.tail().tail().head());
  EXPECT_TRUE(stream.tail().tail().tail().isEmpty());

  ConsStream<int>::iterator i = stream.begin();
  ConsStream<int>::iterator e = stream.end();

  EXPECT_EQ(0, *i);
  i++;
  EXPECT_EQ(1, *i);
  i++;
  EXPECT_EQ(2, *i);
  i++;
  EXPECT_EQ(i, e);

  int k = 0;
  for(auto const& a : stream) {
    EXPECT_EQ(k, a);
    ++k;
  }
}

TEST_F(StreamTest, takeStream) {
  ConsStream<int> from0to4 = rangeFrom(0, 4);
  ConsStream<int> stream = take(from0to4, 3);

  EXPECT_EQ(0, stream.head());
  EXPECT_EQ(1, stream.tail().head());
  EXPECT_EQ(2, stream.tail().tail().head());
  EXPECT_TRUE(stream.tail().tail().tail().isEmpty());

  ConsStream<int>::iterator i = stream.begin();
  ConsStream<int>::iterator e = stream.end();

  EXPECT_EQ(0, *i);
  i++;
  EXPECT_EQ(1, *i);
  i++;
  EXPECT_EQ(2, *i);
  i++;
  EXPECT_EQ(i, e);

  int k = 0;
  for(auto const& a : stream) {
    EXPECT_EQ(k, a);
    ++k;
  }
}

TEST_F(StreamTest, takeInfStream) {
  ConsStream<int> inf = iota(0);
  ConsStream<int> stream = take(inf, 3);

  EXPECT_EQ(0, stream.head());
  EXPECT_EQ(1, stream.tail().head());
  EXPECT_EQ(2, stream.tail().tail().head());
  EXPECT_TRUE(stream.tail().tail().tail().isEmpty());

  ConsStream<int>::iterator i = stream.begin();
  ConsStream<int>::iterator e = stream.end();

  EXPECT_EQ(0, *i);
  i++;
  EXPECT_EQ(1, *i);
  i++;
  EXPECT_EQ(2, *i);
  i++;
  EXPECT_EQ(i, e);

  int k = 0;
  for(auto const& a : stream) {
    EXPECT_EQ(k, a);
    ++k;
  }
}

TEST_F(StreamTest, concatStream) {
  ConsStream<int> inf = iota(0);
  ConsStream<int> s1 = take(inf, 3);
  ConsStream<int> s2 = take(iota(1), 3);
  ConsStream<int> c = append(s1, s2);

  std::vector<int> v{0,1,2,1,2,3};
  int k = 0;
  for(auto const& a : c) {
    EXPECT_EQ(v[k], a);
    ++k;
  }
}

TEST_F(StreamTest, fmapStream) {
  ConsStream<int> inf = iota(0);

  ConsStream<int> square = fmap(inf, [](int i){return i*i;});
  ConsStream<int> front = take(square, 5);

  std::vector<int> v{0,1,4,9,16,25};
  int k = 0;
  for(auto const& a : front) {
    EXPECT_EQ(v[k], a);
    ++k;
  }
  EXPECT_EQ(5, k);

  ConsStream<double> square2 = fmap(inf, [](int i) -> double {return i*i;});
  ConsStream<double> front2 = take(square2, 5);

  std::vector<int> v2{0,1,4,9,16,25};
  int k2 = 0;
  for(auto const& a : front2) {
    EXPECT_EQ(v2[k2], a);
    ++k2;
  }
  EXPECT_EQ(5, k2);
}

TEST_F(StreamTest, countForced) {
  ConsStream<int> inf = iota(0);
  EXPECT_EQ(0, inf.countForced());

  ConsStream<int> take3 = take(inf, 3);
  EXPECT_EQ(0, take3.countForced());

  int k = 0;
  for(auto const& a : take3) {
    EXPECT_EQ(k, a);
    ++k;
  }
  EXPECT_EQ(3, k);
  EXPECT_EQ(3, take3.countForced());
  EXPECT_EQ(3, inf.countForced());

  ConsStream<int> iota3 = iota(3);
  ConsStream<int> inf2 = append(take3, iota3);
  EXPECT_EQ(0, inf2.head());
  EXPECT_EQ(1, inf2.countForced());
  EXPECT_EQ(0, iota3.countForced());
}

TEST_F(StreamTest, concatStreamList) {
  ConsStream<int> inf = iota(0);
  ConsStream<int> s1 = take(inf, 3);
  ConsStream<ConsStream<int>> s2(take(iota(1), 3));
  ConsStream<ConsStream<int>> stream = cons(s1, s2);

  ConsStream<int> c = concat(stream);

  std::vector<int> v{0,1,2,1,2,3};
  int k = 0;
  for(auto const& a : c) {
    EXPECT_EQ(v[k], a);
    ++k;
  }
  EXPECT_EQ(6, k);
}

TEST_F(StreamTest, concatStreamList2) {
  ConsStream<int> inf = iota(0);
  ConsStream<ConsStream<int>> s2 = fmap(inf, [](int i){return rangeFrom(0, i);});
  ConsStream<ConsStream<int>> s3 = take(s2, 3);

  ConsStream<int> c = concat(s3);

  std::vector<int> v{0,0,1,0,1,2,0,1,2,3};
  int k = 0;
  for(auto const& a : c) {
    EXPECT_EQ(v[k], a);
    ++k;
  }
  EXPECT_EQ(6, k);

  EXPECT_EQ(3, inf.countForced());
  EXPECT_EQ(3, s2.countForced());
  EXPECT_EQ(3, s3.countForced());
  EXPECT_EQ(6, c.countForced());

}

TEST_F(StreamTest, joinStreamList) {
  ConsStream<int> inf = iota(0);
  ConsStream<ConsStream<int>> s2 = fmap(inf, [](int i){return rangeFrom(0, i);});
  ConsStream<int> s3 = join(s2);

  ConsStream<int> c = take(s3, 6);

  std::vector<int> v{0,0,1,0,1,2,0,1,2,3};
  int k = 0;
  for(auto const& a : c) {
    EXPECT_EQ(v[k], a);
    ++k;
  }
  EXPECT_EQ(6, k);

  EXPECT_EQ(4, inf.countForced());
  EXPECT_EQ(4, s2.countForced());
  EXPECT_EQ(6, s3.countForced());

}

TEST_F(StreamTest, joinStreamList2) {
  ConsStream<int> inf = iota(0);
  ConsStream<int> s3 = join(fmap(inf, [](int i){return rangeFrom(0, i);}));

  ConsStream<int> c = take(s3, 6);

  std::vector<int> v{0,0,1,0,1,2,0,1,2,3};
  int k = 0;
  for(auto const& a : c) {
    EXPECT_EQ(v[k], a);
    ++k;
  }
  EXPECT_EQ(6, k);

  EXPECT_EQ(4, inf.countForced());
  EXPECT_EQ(6, s3.countForced());

}

TEST_F(StreamTest, bindStreamList) {
  ConsStream<int> inf = iota(0);
  ConsStream<int> s3 = bind(inf,
                            [](int i){return rangeFrom(0, i);});

  EXPECT_EQ(0, s3.head());

  ConsStream<int> c = take(s3, 6);

  std::vector<int> v{0,0,1,0,1,2,0,1,2,3};
  int k = 0;
  for(auto const& a : c) {
    EXPECT_EQ(v[k], a);
    ++k;
  }
  EXPECT_EQ(6, k);

  EXPECT_EQ(4, inf.countForced());
  EXPECT_EQ(6, s3.countForced());

}

TEST_F(StreamTest, thenStreamList) {
  ConsStream<int> inf = iota(0);
  ConsStream<int> s3 = then(inf,
                            [](){return rangeFrom(0, 2);});

  EXPECT_EQ(0, s3.head());

  ConsStream<int> c = take(s3, 6);

  std::vector<int> v{0,1,2,0,1,2,0,1,2,0};
  int k = 0;
  for(auto const& a : c) {
    EXPECT_EQ(v[k], a);
    ++k;
  }
  EXPECT_EQ(6, k);

  EXPECT_EQ(3, inf.countForced());
  EXPECT_EQ(6, s3.countForced());

}

TEST_F(StreamTest, guard) {
  EXPECT_FALSE(guard(true).isEmpty());
  EXPECT_TRUE(guard(false).isEmpty());
}


ConsStream<std::tuple<int, int, int>> triples() {
  return bind(iota(1), [](int z) {
      return bind(rangeFrom(1, z), [z](int x) {
          return bind(rangeFrom(x, z), [x, z](int y) {
              return then(guard(x*x + y*y == z*z), [x, y, z]() {
                  return make_consstream(std::make_tuple(x, y, z));
                });
            });
        });
    });
}

TEST_F(StreamTest, pythag) {
  auto trip = triples();
  EXPECT_EQ(std::make_tuple(3,4,5), trip.head());

  EXPECT_EQ(1, trip.countForced());

  auto tenTrips = take(trip, 10);

  std::tuple<int,int,int> tenth;
  for(auto const& a : tenTrips) {
    tenth = a;
  }
  EXPECT_EQ(std::make_tuple(20,21,29), tenth);
  EXPECT_EQ(10, trip.countForced());

}

TEST_F(StreamTest, strictFuncs) {
  auto from35 = rangeFrom(3,5);
  EXPECT_EQ(5, last(from35));
  auto init35 = init(from35);
  EXPECT_EQ(4, last(init35));
  EXPECT_EQ(3u, length(from35));
  EXPECT_EQ(2u, length(init35));
}

TEST_F(StreamTest, filter) {
  auto inf = iota(1);
  auto even = filter([](int i) {return 0==(i%2);}, inf);
  auto first10 = take(even, 10);
  EXPECT_EQ(2, even.head());
  EXPECT_EQ(20, last(first10));
}

TEST_F(StreamTest, drop) {
  auto inf = iota(3);
  auto drop10 = drop(inf, 10);
  EXPECT_EQ(13, drop10.head());
}

} // end namespace

namespace {
class NoDefault
{
public:
  NoDefault(int);
  NoDefault() = delete;
};

class NoMove
{
public:
  NoMove(NoMove&&) = delete;
  NoMove(NoMove const&){};
  NoMove& operator=(NoMove const&){return *this;};
};
}

template class ConsStreamIterator<int>;
template class ConsStream<std::string>;
template class ConsStream<NoDefault>;
template class ConsStream<NoMove>;
