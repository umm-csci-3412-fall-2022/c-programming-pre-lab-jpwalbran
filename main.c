#include <stdio.h>

#include "check_whitespace.h"

int main() {
  int NUM_STRINGS = 7;
  // Makes an array of 7 string constants for testing.
  char const *strings[] = {
    "Morris",
    "  stuff",
    "Minnesota",
    "nonsense  ",
    "USA",
    "   ",
    "     silliness    "
  };

  for (int i = 0; i < NUM_STRINGS; ++i) {
    if (is_clean(strings[i])) {
      printf("The string '%s' is clean.\n", strings[i]);
    } else {
      printf("The string '%s' is NOT clean.\n", strings[i]);
    }
  }

  return 0;
}
