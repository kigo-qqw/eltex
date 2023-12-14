#include <s_students/s_student_table.h>

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <s_common/s_common.h>
#include <s_students/s_student.h>
#include <s_terminal/s_table_view.h>
#include <s_utils/s_input.h>


struct s_student_table {
  s_student_t_ptr *data;
  size_t size;
  size_t capacity;
};

enum { INT2STR_CONVERT_BUFFER_SIZE = 32 };

static size_t fit_size(size_t const size) {
  if (size == 1) return 0;
  return 1 << (sizeof(size_t) * 8 - __builtin_clzl(size - 1));
}

static void prepare2add(s_student_table_t_ptr const student_table) {
  if (s_student_table_size(student_table) + 1
      > s_student_table_capacity(student_table)) {
    size_t const new_capacity = fit_size(student_table->size + 1);
    student_table->data = realloc(student_table->data, new_capacity);
    student_table->capacity = new_capacity;
  }
}

s_student_table_t_ptr s_student_table_alloc(void) {
  s_student_table_t_ptr const student_table = malloc(sizeof(s_student_table_t));
  if (student_table == NULLPTR) {
    abort();
  }

  student_table->data = NULLPTR;
  student_table->size = 0;
  student_table->capacity = 0;

  return student_table;
}

void s_student_table_init(s_student_table_t_ptr const student_table,
                          size_t capacity) {
  capacity = fit_size(capacity);

  student_table->data = malloc(sizeof(s_student_t_ptr) * capacity);
  if (student_table->data == NULLPTR) {
    abort();
  }
  student_table->size = 0;
  student_table->capacity = capacity;
}

s_student_table_t_ptr s_student_table_create(size_t const capacity) {
  s_student_table_t_ptr const student_table = s_student_table_alloc();
  s_student_table_init(student_table, capacity);
  return student_table;
}

void s_student_table_release(s_student_table_t_ptr const student_table) {
  for (size_t i = 0; i < student_table->size; ++i) {
    s_student_release(student_table->data[i]);
  }
  free(student_table->data);
  free(student_table);
}

inline size_t s_student_table_size(s_student_table_t_ptr const student_table) {
  return student_table->size;
}

inline size_t
s_student_table_capacity(s_student_table_t_ptr const student_table) {
  return student_table->capacity;
}

s_student_table_serialize_error_t
s_student_table_serialize(s_student_table_t_ptr const student_table,
                          FILE *const fs) {
  fprintf(fs, "%lu\n", student_table->size);
  for (size_t i = 0; i < student_table->size; ++i) {
    s_student_serialize_error_t const error =
            s_student_serialize(s_student_table_at(student_table, i), fs);
    if (error == S_SE_OUTPUT_ERR) return ST_SE_OUTPUT_ERR;
  }
  return ST_SE_NO_ERR;
}

s_student_table_deserialize_error_t
s_student_table_deserialize(s_student_table_t_ptr const student_table,
                            FILE *const fs) {
  maybe_int_t maybe_size = input_integer_from_fs(fs);
  if (!maybe_size.is_present) return ST_DSE_PARSE_ERR;
  size_t const size = maybe_size.integer;

  for (size_t i = 0; i < size; ++i) {
    s_student_t_ptr const student = s_student_alloc();
    s_student_deserialize_error_t const error =
            s_student_deserialize(student, fs);
    if (error == S_DSE_INPUT_ERR) return ST_DSE_INPUT_ERR;
    if (error == S_DSE_PARSE_ERR) return ST_DSE_PARSE_ERR;

    s_student_table_add(student_table, student);
  }
  return ST_DSE_NO_ERR;
}

inline s_student_t_ptr
s_student_table_at(s_student_table_t_ptr const student_table,
                   s_student_table_pos_t const index) {
  return student_table->data[index];
}

void s_student_table_set(s_student_table_t_ptr const student_table,
                         s_student_table_pos_t const index,
                         s_student_t_ptr const student) {
  student_table->data[index] = student;
}

s_student_table_pos_t
s_student_table_add(s_student_table_t_ptr const student_table,
                    s_student_t_ptr const student) {
  prepare2add(student_table);
  student_table->data[student_table->size++] = student;
  return student_table->size - 1;
}

s_student_t_ptr
s_student_table_remove(s_student_table_t_ptr const student_table,
                       s_student_table_pos_t const index) {
  s_student_t_ptr const student = student_table->data[index];
  memmove(&student_table->data[index], &student_table->data[index + 1],
          (student_table->size - index - 1) * sizeof(s_student_t_ptr));
  student_table->size--;
  return student;
}

static int student_compare_ints(s_student_t_ptr const *const lhs,
                                s_student_t_ptr const *const rhs) {
  return (s_student_get_id(*lhs) > s_student_get_id(*rhs))
       - (s_student_get_id(*lhs) < s_student_get_id(*rhs));
}

static int student_compare_by_surname(s_student_t_ptr const *const lhs,
                                      s_student_t_ptr const *const rhs) {
  return strcmp(s_student_get_surname(*lhs).data,
                s_student_get_surname(*rhs).data);
}
static int student_compare_by_faculty(s_student_t_ptr const *const lhs,
                                      s_student_t_ptr const *const rhs) {
  return strcmp(s_student_get_faculty(*lhs).data,
                s_student_get_faculty(*rhs).data);
}
static int student_compare_by_group(s_student_t_ptr const *const lhs,
                                    s_student_t_ptr const *const rhs) {
  return strcmp(s_student_get_group(*lhs).data, s_student_get_group(*rhs).data);
}

void s_student_table_sort(s_student_table_t_ptr const student_table,
                          s_student_table_field_t const sort_by) {
  __compar_fn_t compar_fn = NULLPTR;
  switch (sort_by) {
    case ST_FIELD_SURNAME: {
      compar_fn = (__compar_fn_t)student_compare_by_surname;
      break;
    }
    case ST_FIELD_FACULTY: {
      compar_fn = (__compar_fn_t)student_compare_by_faculty;
      break;
    }
    case ST_FIELD_GROUP: {
      compar_fn = (__compar_fn_t)student_compare_by_group;
      break;
    }
    case ST_FIELD_ID: {
      compar_fn = (__compar_fn_t)student_compare_ints;
      break;
    }
  }

  qsort(student_table->data, student_table->size, sizeof(*student_table->data),
        compar_fn);
}

int s_student_table_count_students_in_group(
        s_student_table_t_ptr const student_table, s_string_t const group) {
  int amount = 0;
  for (size_t i = 0; i < student_table->size; ++i) {
    if (strcmp(s_student_get_group(student_table->data[i]).data, group.data)
        == 0) {
      amount++;
    }
  }
  return amount;
}

static size_t levenshtein(char const *const string1,
                          char const *const string2) {
  size_t const string1_len = strlen(string1);
  size_t const string2_len = strlen(string2);
  size_t *cache = calloc(string1_len + 1, sizeof(size_t));

#define MIN3(a, b, c)                                                          \
  ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

  for (size_t y = 1; y <= string1_len; ++y) cache[y] = y;
  for (size_t x = 1; x <= string2_len; ++x) {
    cache[0] = x;
    for (size_t y = 1, lastdiag = x - 1; y <= string1_len; ++y) {
      size_t const olddiag = cache[y];
      cache[y] = MIN3(cache[y] + 1, cache[y - 1] + 1,
                      lastdiag + (string1[y - 1] == string2[x - 1] ? 0 : 1));
      lastdiag = olddiag;
    }
  }

  size_t const result = cache[string1_len];
  free(cache);
  return result;
}

s_student_t_ptr
s_student_table_find_by(s_student_table_t_ptr const student_table,
                        s_student_table_field_t const find_by,
                        void const *const field) {
  if (student_table == NULLPTR || field == NULLPTR) abort();

  s_student_t_ptr student = student_table->data[0];

#define GENERATE_CASE(field_name, field_accessor_func)                         \
  case (field_name): {                                                         \
    size_t levenshtein_value = SIZE_MAX;                                       \
    s_string_t const string = *(s_string_t const *)field;                      \
    for (size_t i = 0; i < student_table->size; ++i) {                         \
      size_t new_levenshtein_value = levenshtein(                              \
              string.data, field_accessor_func(student_table->data[i]).data);  \
      if (new_levenshtein_value < levenshtein_value) {                         \
        levenshtein_value = new_levenshtein_value;                             \
        puts("found");                                                         \
        student = student_table->data[i];                                      \
      }                                                                        \
    }                                                                          \
    break;                                                                     \
  }

  switch (find_by) {
    GENERATE_CASE(ST_FIELD_SURNAME, s_student_get_surname)
    GENERATE_CASE(ST_FIELD_FACULTY, s_student_get_faculty)
    GENERATE_CASE(ST_FIELD_GROUP, s_student_get_group)
    case ST_FIELD_ID: {
      size_t error = SIZE_MAX;
      s_id_t const id = *(s_id_t const *)field;
      for (size_t i = 0; i < student_table->size; ++i) {
        size_t const new_error =
                abs(s_student_get_id(student_table->data[i]) - id);
        if (new_error < error) {
          error = new_error;
          student = student_table->data[i];
        }
      }
      break;
    }
  }

  return student;
}


enum { COLUMN_AMOUNT = 5 };
static char const *const headers[COLUMN_AMOUNT] = {"#", "Surname", "ID",
                                                   "Faculty", "Group"};
static int const column_widht_percent[COLUMN_AMOUNT] = {10, 25, 15, 25, 25};

static s_table_view_data_width_t_ptr table_view_data_width = NULLPTR;

void s_student_table_init_fprint(void) {
  table_view_data_width = s_table_view_data_width_create(COLUMN_AMOUNT, headers,
                                                         column_widht_percent);
}

void s_student_table_release_fprint(void) {
  s_table_view_data_width_release(table_view_data_width);
}

void s_student_table_fprint(s_student_table_t_ptr const student_table,
                            FILE *const fs) {
  if (table_view_data_width == NULLPTR) {
    fprintf(stderr, "init table view before usage (call "
                    "s_student_table_init_fprint())");
    exit(EXIT_FAILURE);
  }

  s_table_view_draw_top_line_to_fs(table_view_data_width, fs);
  s_table_view_draw_header_to_fs(table_view_data_width, fs);

  for (size_t i = 0; i < student_table->size; ++i) {
    s_table_view_draw_mid_line_to_fs(table_view_data_width, fs);

    s_student_t_ptr const student = s_student_table_at(student_table, i);

    char index_buffer[INT2STR_CONVERT_BUFFER_SIZE];
    snprintf(index_buffer, INT2STR_CONVERT_BUFFER_SIZE, "%zu", i);

    char id_buffer[INT2STR_CONVERT_BUFFER_SIZE];
    snprintf(id_buffer, INT2STR_CONVERT_BUFFER_SIZE, "%d",
             s_student_get_id(student));

    char const *const content[COLUMN_AMOUNT] = {
            index_buffer, s_student_get_surname(student).data, id_buffer,
            s_student_get_faculty(student).data,
            s_student_get_group(student).data};

    s_table_view_content_t const table_view_content = {
            .table_view_data_width = table_view_data_width, .content = content};

    s_table_view_draw_content_to_fs(table_view_content, fs);
  }

  s_table_view_draw_bottom_line_to_fs(table_view_data_width, fs);

  (void)student_table;
}

inline void s_student_table_print(s_student_table_t_ptr const student_table) {
  s_student_table_fprint(student_table, stdout);
}
