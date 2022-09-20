#include <gtest/gtest.h>

#include "check_whitespace.h"

TEST(strip, EmptyString) {
    ASSERT_STREQ("", strip(""));
}

TEST(strip, NoWhitespace) {
    ASSERT_STREQ("frog", strip("frog"));
}

TEST(strip, WhitespaceOnFront) {
    ASSERT_STREQ("frog", strip("   frog"));
}

TEST(strip, WhitespaceOnBack) {
    ASSERT_STREQ("frog", strip("frog  "));
}

TEST(strip, WhitespaceOnBothEnds) {
    ASSERT_STREQ("frog", strip("  frog     "));
}

TEST(is_clean, EmptyString) {
    ASSERT_TRUE(is_clean(""));
}

TEST(is_clean, NoWhitespace) {
    ASSERT_TRUE(is_clean("University of Minnesota Morris"));
}

TEST(is_clean, WhitespaceOnFront) {
    ASSERT_FALSE(is_clean("   University of Minnesota Morris"));
}

TEST(is_clean, WhitespaceOnBack) {
    ASSERT_FALSE(is_clean("University of Minnesota Morris  "));
}

TEST(is_clean, WhitespaceOnBothEnds) {
    ASSERT_FALSE(is_clean(" University of Minnesota Morris"    ));
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}