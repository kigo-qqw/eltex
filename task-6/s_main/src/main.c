#include <s_common/s_common.h>
#include <s_students/s_student_table.h>
#include <s_utils/s_input.h>
#include <stdlib.h>
#include <unistd.h>


enum { STUDENT_TABLE_INIT_CAPACITY = 16 };

static void show_help() {
  puts("\n"
       "[a]dd student\n"
       "[s]ave students table\n"
       "[l]oad students table\n"
       "[v]iew students table\n"
       "s[o]rt students table by field\n"
       "[f]ind student by field\n"
       "[d]elete student\n"
       "[e]dit student\n"
       "[c]ount students in group\n"
       "[h]elp\n"
       "[q]uit\n");
}

char input_character() {
  // getchar работает странно при использовании fgets для получения полей
  // студента
  char buf[128];
  const char *const a = fgets(buf, 128, stdin);
  if (a == NULLPTR || *a == '\n') return '\0';
  return *a;
}

int main(int const argc, char const *const argv[]) {
  (void)argc;
  (void)argv;
  s_student_table_init_fprint();

  bool run = true;
  s_student_table_t_ptr student_table =
          s_student_table_create(STUDENT_TABLE_INIT_CAPACITY);

  while (run) {
    show_help();
    char const c = input_character();
    switch (c) {
      case 'a': {
        s_student_t_ptr const student = s_student_alloc();
        s_student_init_from_keyboard(student);

        s_student_table_add(student_table, student);
        break;
      }
      case 's': {
        maybe_s_string_t const maybe_filename =
                input_line_from_stdin_with_prompt_to_stdout("filename: ");
        if (maybe_filename.is_present) {
          FILE *const fs = fopen(maybe_filename.str.data, "w");
          s_student_table_serialize(student_table, fs);
          fclose(fs);

          s_string_release(maybe_filename.str);
        }
        break;
      }
      case 'l': {
        maybe_s_string_t const maybe_filename =
                input_line_from_stdin_with_prompt_to_stdout("filename: ");
        if (maybe_filename.is_present) {
          FILE *const fs = fopen(maybe_filename.str.data, "r");
          if (student_table != NULLPTR) {
            s_student_table_release(student_table);
          }
          student_table = s_student_table_create(STUDENT_TABLE_INIT_CAPACITY);
          s_student_table_deserialize(student_table, fs);
          fclose(fs);

          s_string_release(maybe_filename.str);
        }
        break;
      }
      case 'v': {
        s_student_table_print(student_table);
        break;
      }
      case 'o': {
        puts("sort by [s]urname\n"
             "        [i]d\n"
             "        [f]aculty\n"
             "        [g]roup\n");
        char const choice = input_character();
        s_student_table_field_t sort_by = 0;
        bool is_error = false;

        switch (choice) {
          case 's': {
            sort_by = ST_FIELD_SURNAME;
            break;
          }
          case 'i': {
            sort_by = ST_FIELD_ID;
            break;
          }
          case 'f': {
            sort_by = ST_FIELD_FACULTY;
            break;
          }
          case 'g': {
            sort_by = ST_FIELD_GROUP;
            break;
          }
          default: {
            is_error = true;
            puts("invalid choice");
            break;
          }
        }
        if (!is_error) {
          s_student_table_sort(student_table, sort_by);
          s_student_table_print(student_table);
        }
        break;
      }
      case 'f': {
        puts("find by [s]urname\n"
             "        [i]d\n"
             "        [f]aculty\n"
             "        [g]roup\n");
        char const choice = input_character();
        s_student_table_field_t find_by = 0;
        bool is_error = false;

        switch (choice) {
          case 's': {
            find_by = ST_FIELD_SURNAME;
            break;
          }
          case 'i': {
            find_by = ST_FIELD_ID;
            break;
          }
          case 'f': {
            find_by = ST_FIELD_FACULTY;
            break;
          }
          case 'g': {
            find_by = ST_FIELD_GROUP;
            break;
          }
          default: {
            is_error = true;
            puts("invalid choice");
            break;
          }
        }
        if (!is_error) {
          maybe_s_string_t maybe_field =
                  input_line_from_stdin_with_prompt_to_stdout("field value: ");
          if (maybe_field.is_present) {
            s_student_t_ptr student = NULLPTR;

            if (find_by == ST_FIELD_ID) {
              int const id = strtol(maybe_field.str.data, NULL, 10);
              student = s_student_table_find_by(student_table, find_by, &id);
            } else {
              student = s_student_table_find_by(student_table, find_by,
                                                &maybe_field.str);
            }

            s_student_table_t_ptr const student_table_tmp = s_student_table_create(2);
            s_student_table_add(student_table_tmp, s_student_clone(student));
            s_student_table_print(student_table_tmp);
            s_student_table_release(student_table_tmp);

            s_string_release(maybe_field.str);
          }
        }
        break;
      }
      case 'd': {
        maybe_int_t const maybe_index =
                input_integer_from_stdin_with_prompt_to_stdout("index: ");
        if (maybe_index.is_present) {
          s_student_t_ptr const student =
                  s_student_table_remove(student_table, maybe_index.integer);
          s_student_release(student);
        }
        break;
      }
      case 'e': {
        maybe_int_t const maybe_index =
                input_integer_from_stdin_with_prompt_to_stdout("index: ");
        if (maybe_index.is_present) {
          s_student_t_ptr const old_student =
                  s_student_table_at(student_table, maybe_index.integer);

          s_student_t_ptr const student = s_student_alloc();
          s_student_init_from_keyboard(student);
          s_student_table_set(student_table, maybe_index.integer, student);

          s_student_release(old_student);
        }
        break;
      }
      case 'c': {
        maybe_s_string_t const maybe_group =
                input_line_from_stdin_with_prompt_to_stdout("group: ");
        if (maybe_group.is_present) {
          int const amount = s_student_table_count_students_in_group(
                  student_table, maybe_group.str);
          printf("number of students in group: %d\n", amount);
          s_string_release(maybe_group.str);
        }
        break;
      }
      case 'q': {
        run = false;
        break;
      }
      case 'h':
      default: {
        show_help();
        break;
      }
    }
  }
  s_student_table_release(student_table);
  s_student_table_release_fprint();
  return 0;
}
