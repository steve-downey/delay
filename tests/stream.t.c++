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
    ConsStream<int> cs2(1);

    EXPECT_TRUE(cs1.isEmpty());
    EXPECT_FALSE(cs2.isEmpty());

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

ConsStream<int> rangeFrom(int n, int m) {
  if (n > m) {
    return ConsStream<int>();
  }
  return ConsStream<int>([n, m]() {
      return ConsCell<int>(n, rangeFrom(n+1, m));
    });
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


}

template class ConsStreamIterator<int>;
