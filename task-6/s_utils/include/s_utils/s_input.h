#pragma once

#include <stdio.h>

#include <s_string/s_string.h>

typedef struct {
  s_string_t str;
  bool is_present;
} maybe_s_string_t;

/**
 * \brief Ввод строки из потока с предварительным сообщением (опционально)
 * \param in Поток ввода
 * \param out Поток вывода (может быть nullptr)
 * \param prompt Сообщение (nullptr, если out == nullptr)
 * \return
 */
maybe_s_string_t input_line_from_fs_with_prompt_to_fs(FILE *in, FILE *out,
                                                      const char *prompt);
maybe_s_string_t input_line_from_fs(FILE *fs);
maybe_s_string_t
input_line_from_stdin_with_prompt_to_stdout(char const *prompt);

typedef struct {
  int integer;
  bool is_present;
} maybe_int_t;

maybe_int_t input_integer_from_fs_with_prompt_to_fs(FILE *in, FILE *out,
                                                    const char *prompt);
maybe_int_t input_integer_from_fs(FILE *fs);
maybe_int_t
input_integer_from_stdin_with_prompt_to_stdout(char const *prompt);
