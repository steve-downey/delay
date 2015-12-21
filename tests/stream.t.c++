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
  ConsStream<int> c = concat(s1, s2);

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

  ConsStream<double> square2 = fmap(inf, [](int i) -> double {return i*i;});
  ConsStream<double> front2 = take(square2, 5);

  std::vector<int> v2{0,1,4,9,16,25};
  int k2 = 0;
  for(auto const& a : front2) {
    EXPECT_EQ(v2[k2], a);
    ++k2;
  }

}
}

template class ConsStreamIterator<int>;
