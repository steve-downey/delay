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

    //    ConsCell<int> cc1;
    ConsCell<int> cc2(1);

    ConsStream<int> cs1;
    ConsStream<int> cs2(1);

    EXPECT_TRUE(cs1.isEmpty());
    EXPECT_FALSE(cs2.isEmpty());

    // int i = -1;
    // EXPECT_THROW({
    //         i = cc1.head();
    //     }, std::experimental::bad_optional_access);

    // EXPECT_EQ(-1, i);

    int j = -1;
    EXPECT_NO_THROW({
            j = cc2.head();
        });
    EXPECT_EQ(1, j);

    // ConsStream<int> str1 = cc1.tail();
    ConsStream<int> str2 = cc2.tail();

    // EXPECT_TRUE(str1.isEmpty());
    EXPECT_TRUE(str2.isEmpty());

    int k = cs2.head();
    ConsStream<int> str3 = cs2.tail();

    EXPECT_EQ(1, k);
    EXPECT_TRUE(str3.isEmpty());

}
}
