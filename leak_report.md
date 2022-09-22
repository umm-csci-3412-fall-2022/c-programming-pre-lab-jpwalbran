# Leak report

### Memory leaks in `./check_whitespace.c`

Running the valgrind analysis on `./check_whitespace.c` yields the following output:
```
==11746== Memcheck, a memory error detector
==11746== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==11746== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==11746== Command: ./check_whitespace
==11746==
The string 'Morris' is clean.
The string '  stuff' is NOT clean.
The string 'Minnesota' is clean.
The string 'nonsense  ' is NOT clean.
The string 'USA' is clean.
The string '   ' is NOT clean.
The string '     silliness    ' is NOT clean.
==11746==
==11746== HEAP SUMMARY:
==11746==     in use at exit: 46 bytes in 6 blocks
==11746==   total heap usage: 7 allocs, 1 frees, 1,070 bytes allocated
==11746==
==11746== 46 bytes in 6 blocks are definitely lost in loss record 1 of 1
==11746==    at 0x4C3BE4B: calloc (vg_replace_malloc.c:1328)
==11746==    by 0x4007BB: strip (check_whitespace.c:36)
==11746==    by 0x400827: is_clean (check_whitespace.c:58)
==11746==    by 0x4006C7: main (main.c:19)
==11746==
==11746== LEAK SUMMARY:
==11746==    definitely lost: 46 bytes in 6 blocks
==11746==    indirectly lost: 0 bytes in 0 blocks
==11746==      possibly lost: 0 bytes in 0 blocks
==11746==    still reachable: 0 bytes in 0 blocks
==11746==         suppressed: 0 bytes in 0 blocks
==11746==
==11746== For lists of detected and suppressed errors, rerun with: -s
==11746== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

This suggests that the memory leak occurs because we call the `is_clean()` method.
If we follow the stack trace back, we see that the `is_clean()` method contains the following code:
```c
/*
 * Return true (1) if the given string is "clean", i.e., has
 * no spaces at the front or the back of the string.
*/
int is_clean(char const *str) {
  // We check if it's clean by calling strip and seeing if the
  // result is the same as the original string.
  char const *cleaned = strip(str);

  // strcmp compares two strings, returning a negative value if
  // the first is less than the second (in alphabetical order),
  // 0 if they're equal, and a positive value if the first is
  // greater than the second.
  int result = strcmp(str, cleaned);

  return result == 0;
}
```

Considering we're looking for a memory leak, we are looking for any reference to a pointer that doesn't have
an associated `free()` statement with it. (Or, lacking that, some value that contains a pointer that has internal pointers)
In this case, the relevant line here is the line defining cleaned, due to the fact that it dereferences the output of the `strip()` command.

Therefore, since there are no actual pointer uses here, the memory leak is most likely instead in the `strip()` function.
The code of the `strip()` function is as follows:
```c
/*
 * Strips spaces from both the front and back of a string,
 * leaving any internal spaces alone.
 */
char const *strip(char const *str) {
  int size = strlen(str);

  // This counts the number of leading and trailing spaces
  // so we can figure out how big the result array should be.
  int num_spaces = 0;
  int first_non_space = 0;
  while (first_non_space < size && str[first_non_space] == ' ')
    ++num_spaces;
    ++first_non_space;
  }

  int last_non_space = size-1;
  while (last_non_space >= 0 && str[last_non_space] == ' ') {
    ++num_spaces;
    --last_non_space;
  }

  // If num_spaces >= size then that means that the string
  // consisted of nothing but spaces, so we'll return the
  // empty string.
  if (num_spaces >= size) {
    return "";
  }

  // Allocate a slot for all the "saved" characters
  // plus one extra for the null terminator.
  char* result = (char*) calloc(size-num_spaces+1, sizeof(char))

  // Copy in the "saved" characters.
  int i;
  for (i = first_non_space; i <= last_non_space; ++i) {
    result[i-first_non_space] = str[i];
  }
  // Place the null terminator at the end of the result string.
  result[i-first_non_space] = '\0';

  return result;
}
```

If we look at the above, we notice that there is a pointer variable, `*result` that is never freed. So we would want to start by
attempting to free the variable `*result` before the return.


Therefore, we should try to add `free(result)` the the function one line above the return statement.
 > As a note, this doesn't actually change the memory address, so in this case,
 > we're able to free the memory, and then we read the value of the pointer.
 > It's often advised to set the pointer value to NULL after wee free the memory, but in this case
 > since we're returning from the function right after freeing the variable, it should be fine to not
 > do so in this case.

 After testing this, this turned out to be wildly incorrect, because we were freeing memory that we needed to read from.
 Instead, it was doable to free the memory in the `is_clean()' method. There, we wanted to free the memory referenced by the 'cleaned' pointer. In order to do this, we needed to cast it to a void pointer.
 This, however, also didn't quite work, because the `strip()` method doesn't always allocate memory, (If the string is all spaces,
 then the method just returns). Therefore, we need to add a check that the string to return wasn't empty. (This meant checking that the length of `cleaned` was greater than 0). After doing this, the valgrind and other checks were all happy.
