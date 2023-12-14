#pragma once

#include <stdio.h>

typedef struct s_table_view_data_width s_table_view_data_width_t;
typedef s_table_view_data_width_t *s_table_view_data_width_t_ptr;

typedef struct s_table_view_content {
  s_table_view_data_width_t_ptr table_view_data_width;
  char const *const *content;
} s_table_view_content_t;


s_table_view_data_width_t_ptr
s_table_view_data_width_create(size_t column_amount, char const *const *headers,
                               int const *column_width_percent);
void s_table_view_data_width_release(
        s_table_view_data_width_t_ptr table_view_data_width);

void s_table_view_draw_top_line_to_fs(
        s_table_view_data_width_t_ptr table_view_data, FILE *fs);
void s_table_view_draw_mid_line_to_fs(
        s_table_view_data_width_t_ptr table_view_data, FILE *fs);
void s_table_view_draw_bottom_line_to_fs(
        s_table_view_data_width_t_ptr table_view_data, FILE *fs);
void s_table_view_draw_content_to_fs(s_table_view_content_t table_view_content,
                                     FILE *fs);
void s_table_view_draw_header_to_fs(
        s_table_view_data_width_t_ptr table_view_data, FILE *fs);

void s_table_view_draw_top_line(s_table_view_data_width_t_ptr table_view_data);
void s_table_view_draw_mid_line(s_table_view_data_width_t_ptr table_view_data);
void s_table_view_draw_bottom_line(
        s_table_view_data_width_t_ptr table_view_data);
void s_table_view_draw_content(s_table_view_content_t table_view_content);
void s_table_view_draw_header(s_table_view_data_width_t_ptr table_view_data);
