#include <s_utils/s_input.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <s_common/s_common.h>


enum { INPUT_BUFFER_SIZE = 1024 };

bool validate_integer(const char *str);

maybe_s_string_t
input_line_from_fs_with_prompt_to_fs(FILE *const in, FILE *const out,
                                     const char *const prompt) {
  if (out != NULLPTR) fputs(prompt, out);

  char buffer[INPUT_BUFFER_SIZE];
  char const *const input_str = fgets(buffer, INPUT_BUFFER_SIZE, in);
  buffer[strcspn(buffer, "\n")] = '\0';

  maybe_s_string_t result = {.str = {NULLPTR}, .is_present = false};

  if (input_str != NULLPTR) {
    result.str = s_string_create(input_str);
    result.is_present = true;
  }
  return result;
}

maybe_s_string_t input_line_from_fs(FILE *fs) {
  return input_line_from_fs_with_prompt_to_fs(fs, NULLPTR, NULLPTR);
}

maybe_s_string_t
input_line_from_stdin_with_prompt_to_stdout(char const *const prompt) {
  return input_line_from_fs_with_prompt_to_fs(stdin, stdout, prompt);
}

maybe_int_t input_integer_from_fs_with_prompt_to_fs(FILE *in, FILE *out,
                                                    const char *prompt) {
  maybe_int_t result = {.integer = 0, .is_present = false};

  maybe_s_string_t maybe_string =
          input_line_from_fs_with_prompt_to_fs(in, out, prompt);
  if (maybe_string.is_present) {
    if (validate_integer(maybe_string.str.data)) {
      int const integer = strtol(maybe_string.str.data, NULL, 10);
      if (errno == 0) {
        result.integer = integer;
        result.is_present = true;
      }
    }
    s_string_release(maybe_string.str);
    return result;
  }
  return result;
}
maybe_int_t input_integer_from_fs(FILE *fs) {
  return input_integer_from_fs_with_prompt_to_fs(fs, NULLPTR, NULLPTR);
}
maybe_int_t input_integer_from_stdin_with_prompt_to_stdout(char const *prompt) {
  return input_integer_from_fs_with_prompt_to_fs(stdin, stdout, prompt);
}

bool validate_integer(const char *const str) {
  for (char const *c = str; *c != '\0'; ++c) {
    if (*c < '0' || *c > '9') return false;
  }
  return true;
}
