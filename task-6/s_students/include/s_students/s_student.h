#pragma once

#include <stdio.h>

#include <s_string/s_string.h>

typedef s_string_t s_surname_t;
typedef int s_id_t;
typedef s_string_t s_faculty_t;
typedef s_string_t s_group_t;

typedef struct s_student s_student_t;
typedef s_student_t *s_student_t_ptr;

typedef enum { S_SE_NO_ERR, S_SE_OUTPUT_ERR } s_student_serialize_error_t;
typedef enum {
  S_DSE_NO_ERR,
  S_DSE_INPUT_ERR,
  S_DSE_PARSE_ERR
} s_student_deserialize_error_t;
typedef enum {
  S_P_NO_ERR,
  S_P_INPUT_ERR,
  S_P_PARSE_ERR
} s_student_parse_error_t;

/**
 * \brief Выделения памяти для студента
 * \return
 */
s_student_t_ptr s_student_alloc(void);

/**
 * \brief Инициализация
 * \param student
 * \param surname Фамилия студента, передача владения структуре студента
 * \param id Номер зачетной книжки студента
 * \param faculty Факультет студента, передача владения структуре студента
 * \param group Группа студента, передача владения структуре студента
 */
void s_student_init(s_student_t_ptr student, s_surname_t surname, s_id_t id,
                    s_faculty_t faculty, s_group_t group);

/**
 * \brief Создание студента (alloc + init)
 * \param surname Фамилия студента, передача владения структуре студента
 * \param id Номер зачетной книжки студента
 * \param faculty Факультет студента, передача владения структуре студента
 * \param group Группа студента, передача владения структуре студента
 * \return
 */
s_student_t_ptr s_student_create(s_surname_t surname, s_id_t id,
                                 s_faculty_t faculty, s_group_t group);

/**
 * \brief Клонирование студента
 * \param student
 * \return
 */
s_student_t_ptr s_student_clone(s_student_t_ptr student);

/**
 * \brief Удаление студента
 * \param student
 */
void s_student_release(s_student_t_ptr student);

s_surname_t s_student_get_surname(s_student_t_ptr student);
s_id_t s_student_get_id(s_student_t_ptr student);
s_faculty_t s_student_get_faculty(s_student_t_ptr student);
s_group_t s_student_get_group(s_student_t_ptr student);

/**
 * \brief Сериализация студента в поток
 * \param student
 * \param fs Поток для записи
 * \return Ошибка
 */
s_student_serialize_error_t s_student_serialize(s_student_t_ptr student, FILE * fs);

/**
 * \brief Десериализация студента в поток
 * \param student
 * \param fs Поток для чтения
 * \return Ошибка
 */
s_student_deserialize_error_t s_student_deserialize(s_student_t_ptr student,FILE * fs);

/**
 * \brief Инициализация студента с потока ввода
 * \param student
 * \return
 */
s_student_parse_error_t s_student_init_from_keyboard(s_student_t_ptr student);
