#pragma once

#include <stdio.h>

#include <s_students/s_student.h>

typedef struct s_student_table s_student_table_t;
typedef s_student_table_t *s_student_table_t_ptr;
typedef int s_student_table_pos_t;
typedef enum {
  ST_SE_NO_ERR,
  ST_SE_OUTPUT_ERR
} s_student_table_serialize_error_t;
typedef enum {
  ST_DSE_NO_ERR,
  ST_DSE_INPUT_ERR,
  ST_DSE_PARSE_ERR
} s_student_table_deserialize_error_t;
typedef enum {
  ST_FIELD_SURNAME,
  ST_FIELD_ID,
  ST_FIELD_FACULTY,
  ST_FIELD_GROUP
} s_student_table_field_t;

/**
 * \brief Выделения памяти для таблицы студентов
 * \return
 */
s_student_table_t_ptr s_student_table_alloc(void);

/**
 * \brief Инициализация
 * \param student_table
 * \param capacity Количество элементов, для которых будет выделено место
 */
void s_student_table_init(s_student_table_t_ptr student_table, size_t capacity);

/**
 * \brief Создание таблицы студентов (alloc + init)
 * \param capacity Количество элементов, для которых будет выделено место
 * \return
 */
s_student_table_t_ptr s_student_table_create(size_t capacity);
/**
 * \brief Удаление таблицы студентов
 * \param student_table
 */
void s_student_table_release(s_student_table_t_ptr student_table);

/**
 * \brief Количество студентов в таблице
 * \param student_table
 * \return
 */
size_t s_student_table_size(s_student_table_t_ptr student_table);
/**
 * \brief Количество студентов, для которых выделено место
 * \param student_table
 * \return
 */
size_t s_student_table_capacity(s_student_table_t_ptr student_table);

/**
 * \brief Сериализация таблицы в поток
 * \param student_table
 * \param fs Поток для записи
 * \return Ошибка
 */
s_student_table_serialize_error_t
s_student_table_serialize(s_student_table_t_ptr student_table, FILE *fs);
/**
 * \brief Десериализация таблицы из потока
 * \param student_table
 * \param fs Поток для чтений
 * \return Ошибка
 */
s_student_table_deserialize_error_t
s_student_table_deserialize(s_student_table_t_ptr student_table, FILE *fs);

/**
 * \brief Получение студента по индексу в таблице
 * \param student_table
 * \param index Индекс студента в таблице
 * \return
 */
s_student_t_ptr s_student_table_at(s_student_table_t_ptr student_table,
                                   s_student_table_pos_t index);

/**
 * \brief Устанавливает студента по индексу в таблице
 * \param student_table
 * \param index Индекс студента в таблице
 * \param student Студент
 */
void s_student_table_set(s_student_table_t_ptr student_table,
                         s_student_table_pos_t index, s_student_t_ptr student);
/**
 * \brief Добавление студента в конец таблицы
 * \param student_table
 * \param student
 * \return
 */
s_student_table_pos_t s_student_table_add(s_student_table_t_ptr student_table,
                                          s_student_t_ptr student);

/**
 * \brief Удаление студента по индексу
 * \param student_table
 * \param index Индекс студента для удаления
 * \return Удаленный студент
 */
s_student_t_ptr s_student_table_remove(s_student_table_t_ptr student_table,
                                       s_student_table_pos_t index);

/**
 * \brief Сортировка таблицы студентов по полю (вполне вероятно стоит принимать
 * предикат нежели перечисление)
 * \param student_table
 * \param sort_by Поле по которому происходит сортировка
 */
void s_student_table_sort(s_student_table_t_ptr student_table,
                          s_student_table_field_t sort_by);

/**
 * \brief Подсчет количества студентов в группе
 * \param student_table
 * \param group Группа
 * \return
 */
int s_student_table_count_students_in_group(s_student_table_t_ptr student_table,
                                            s_string_t group);

/**
 * \brief Поиск по таблице студентов по полю (вполне вероятно стоит принимать функцию оценки нежели перечисление и поле)
 * \param student_table
 * \param find_by
 * \param field
 * \return
 */
s_student_t_ptr s_student_table_find_by(s_student_table_t_ptr student_table,
  s_student_table_field_t find_by,
  void const * field);

void s_student_table_init_fprint(void);
void s_student_table_release_fprint(void);

/**
 * \brief Вывод таблицы в поток
 * \param student_table
 * \param fs Поток вывода
 */
void s_student_table_fprint(s_student_table_t_ptr student_table,FILE*fs);

/**
 * \brief Вывод таблицы
 * \param student_table
 */
void s_student_table_print(s_student_table_t_ptr student_table);
