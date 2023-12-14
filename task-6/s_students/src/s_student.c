#include <s_students/s_student.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <s_common/s_common.h>
#include <s_utils/s_input.h>


struct s_student {
  s_surname_t surname;
  s_id_t id;
  s_faculty_t faculty;
  s_group_t group;
};

s_student_t_ptr s_student_alloc(void) {
  s_student_t_ptr const student = malloc(sizeof(s_student_t));
  if (student == NULLPTR) {
    abort();
  }
  return student;
}

void s_student_init(s_student_t_ptr const student, s_surname_t const surname,
                    s_id_t const id, s_faculty_t const faculty,
                    s_group_t const group) {
  student->surname = surname;
  student->id = id;
  student->faculty = faculty;
  student->group = group;
}

s_student_t_ptr s_student_create(s_surname_t const surname, s_id_t const id,
                                 s_faculty_t const faculty,
                                 s_group_t const group) {
  s_student_t_ptr const student = s_student_alloc();
  s_student_init(student, surname, id, faculty, group);
  return student;
}
s_student_t_ptr s_student_clone(s_student_t_ptr const student) {
  return s_student_create(s_string_create(student->surname.data), student->id,
                          s_string_create(student->faculty.data),
                          s_string_create(student->group.data));
}

void s_student_release(s_student_t_ptr const student) {
  s_string_release(student->surname);
  s_string_release(student->faculty);
  s_string_release(student->group);
  free(student);
}

s_surname_t s_student_get_surname(s_student_t_ptr const student) {
  return student->surname;
}
s_id_t s_student_get_id(s_student_t_ptr const student) {
  return student->id;
}
s_faculty_t s_student_get_faculty(s_student_t_ptr const student) {
  return student->faculty;
}
s_group_t s_student_get_group(s_student_t_ptr const student) {
  return student->group;
}

s_student_serialize_error_t s_student_serialize(s_student_t_ptr const student,
                                                FILE *const fs) {
  int const count =
          fprintf(fs, "%s\n%d\n%s\n%s\n", student->surname.data, student->id,
                  student->faculty.data, student->group.data);
  if (count < 0) return S_SE_OUTPUT_ERR;
  return S_SE_NO_ERR;
}

s_student_deserialize_error_t
s_student_deserialize(s_student_t_ptr const student, FILE *const fs) {
#define INPUT_AND_CHECK(var_name)                                              \
  maybe_s_string_t var_name = input_line_from_fs(fs);                          \
  if (!(var_name).is_present) return S_DSE_INPUT_ERR;

  INPUT_AND_CHECK(maybe_surname);
  maybe_int_t maybe_id = input_integer_from_fs(fs);
  if (!maybe_id.is_present) return S_DSE_PARSE_ERR;
  INPUT_AND_CHECK(maybe_faculty);
  INPUT_AND_CHECK(maybe_group);

  student->surname = maybe_surname.str;
  student->id = maybe_id.integer;
  student->faculty = maybe_faculty.str;
  student->group = maybe_group.str;

  return S_DSE_NO_ERR;
}

// Копипаст, т.к. стратегия ввода с клавиатуры может значительно отличаться от
// стратегии десериализации в процессе жизни кода
s_student_parse_error_t
s_student_init_from_keyboard(s_student_t_ptr const student) {
#undef INPUT_AND_CHECK
#define INPUT_AND_CHECK(var_name, prompt)                                      \
  maybe_s_string_t var_name =                                                  \
          input_line_from_stdin_with_prompt_to_stdout((prompt));               \
  if (!(var_name).is_present) return S_P_INPUT_ERR;

  INPUT_AND_CHECK(maybe_surname, "Surname: ");
  maybe_int_t maybe_id = input_integer_from_stdin_with_prompt_to_stdout("ID: ");
  if (!maybe_id.is_present) return S_P_PARSE_ERR;
  INPUT_AND_CHECK(maybe_faculty, "Faculty: ");
  INPUT_AND_CHECK(maybe_group, "Group: ");

  student->surname = maybe_surname.str;
  student->id = maybe_id.integer;
  student->faculty = maybe_faculty.str;
  student->group = maybe_group.str;

  return S_P_NO_ERR;
}

