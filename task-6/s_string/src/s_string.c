#include <s_string/s_string.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <s_common/s_common.h>


inline s_string_t s_string_create(char const *const str) {
  return s_string_create_with_size(str, strlen(str));
}

s_string_t s_string_create_with_size(char const *const str, size_t const size) {
  char *data = calloc(size + 1, sizeof(char));
  if (data == NULLPTR) {
    abort();
  }
  strncpy(data, str, size + 1);
  s_string_t const string = {.data = data};
  return string;
}

inline void s_string_release(s_string_t const string) {
  free(string.data);
}
