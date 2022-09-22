#include <gtest/gtest.h>

#include "check_whitespace.h"

TEST(strip, EmptyString) {
    ASSERT_STREQ("", strip(""));
}

TEST(strip, NoWhitespace) {
    char* const testString = strip("frog");
    ASSERT_STREQ("frog", testString);
    free((void*) testString);
}

TEST(strip, WhitespaceOnFront) {
    char* const testString = strip("    frog");
    ASSERT_STREQ("frog", testString);
    free((void*) testString);
}

TEST(strip, WhitespaceOnBack) {
    char* const testString = strip("frog  ");
    ASSERT_STREQ("frog", testString);
    free(testString);
}

TEST(strip, WhitespaceOnBothEnds) {
    char* const testString = strip("  frog    ");
    ASSERT_STREQ("frog", testString);
    free((void*) testString);
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
