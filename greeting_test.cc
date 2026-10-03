#include "greeting.h"

#include "gtest/gtest.h"

namespace {

TEST(JoinWordsTest, JoinsWithDash) {
  EXPECT_EQ(JoinWords({"foo", "bar", "baz"}), "foo-bar-baz");
}

TEST(JoinWordsTest, EmptyInput) { EXPECT_EQ(JoinWords({}), ""); }

}  // namespace
