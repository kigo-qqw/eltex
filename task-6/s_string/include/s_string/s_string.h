#pragma once

#include <stddef.h>

typedef struct s_string {
  char *data;
} s_string_t;

/**
 * \brief Создание строки из массива символов
 * \param str Строка, которая будет скопирована в новую строку
 * \return
 */
s_string_t s_string_create(char const *str);

/**
 * \brief Создание строки из части массива символов размера `size`
 * \param str Строка, часть которой будет скопирована в новую строку
 * \param size Размер части
 * \return
 */
s_string_t s_string_create_with_size(char const*str, size_t size);

/**
 * \brief Удаление строки
 * \param string
 */
void s_string_release(s_string_t string);
