#include <s_terminal/s_table_view.h>

#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <s_common/s_common.h>

#define TABLE_VIEW_BORDER_CHAR_TOP "─"
#define TABLE_VIEW_BORDER_CHAR_TOPMID "┬"
#define TABLE_VIEW_BORDER_CHAR_TOPLEFT "┌"
#define TABLE_VIEW_BORDER_CHAR_TOPRIGHT "┐"
#define TABLE_VIEW_BORDER_CHAR_BOTTOM "─"
#define TABLE_VIEW_BORDER_CHAR_BOTTOMMID "┴"
#define TABLE_VIEW_BORDER_CHAR_BOTTOMLEFT "└"
#define TABLE_VIEW_BORDER_CHAR_BOTTOMRIGHT "┘"
#define TABLE_VIEW_BORDER_CHAR_LEFT "│"
#define TABLE_VIEW_BORDER_CHAR_MIDLEFT "├"
#define TABLE_VIEW_BORDER_CHAR_MID "─"
#define TABLE_VIEW_BORDER_CHAR_MIDMID "┼"
#define TABLE_VIEW_BORDER_CHAR_RIGHT "│"
#define TABLE_VIEW_BORDER_CHAR_MIDRIGHT "┤"

struct s_table_view_data_width_percent {
  size_t column_amount;
  char const *headers;
  int const *column_width_percent;
};

struct s_table_view_data_width {
  size_t column_amount;
  char const *const *headers;
  int const *column_width;
};


static void draw_line_to_fs(s_table_view_data_width_t_ptr table_view_data_width,
                            FILE *fs, char const *main, char const *left,
                            char const *mid, char const *right);
static void repeat_to_fs(FILE *fs, char const *string, size_t amount);

s_table_view_data_width_t_ptr
s_table_view_data_width_create(size_t const column_amount,
                               char const *const *const headers,
                               int const *const column_width_percent) {
  struct winsize ws;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

  int const terminal_width = ws.ws_col;
  if (terminal_width < 2) {
    fprintf(stderr, "unsupported terminal (terminal width = %d)",
            terminal_width);
    exit(EXIT_FAILURE);
  }

  int const free_terminal_width = terminal_width - 2 - (column_amount - 1);
  int sum = 0;
  s_table_view_data_width_t_ptr const table_view_data_width =
          malloc(sizeof(s_table_view_data_width_t));
  if (table_view_data_width == NULLPTR) {
    abort();
  }

  table_view_data_width->column_amount = column_amount;
  table_view_data_width->headers = headers;
  int *column_width = malloc(column_amount * sizeof(int const));
  if (column_width == NULLPTR) {
    abort();
  }

  for (size_t i = 0; i + 1 < column_amount; ++i) {
    column_width[i] = free_terminal_width * column_width_percent[i] / 100;
    sum += column_width[i];
  }
  column_width[column_amount - 1] = free_terminal_width - sum;

  table_view_data_width->column_width = column_width;
  return table_view_data_width;
}

void s_table_view_data_width_release(
        s_table_view_data_width_t_ptr const table_view_data_width) {
  free((void *)table_view_data_width->column_width);
  free(table_view_data_width);
}

void s_table_view_draw_top_line_to_fs(
        s_table_view_data_width_t_ptr const table_view_data, FILE *const fs) {
  draw_line_to_fs(table_view_data, fs, TABLE_VIEW_BORDER_CHAR_TOP,
                  TABLE_VIEW_BORDER_CHAR_TOPLEFT, TABLE_VIEW_BORDER_CHAR_TOPMID,
                  TABLE_VIEW_BORDER_CHAR_TOPRIGHT);
}
void s_table_view_draw_mid_line_to_fs(
        s_table_view_data_width_t_ptr const table_view_data, FILE *const fs) {
  draw_line_to_fs(table_view_data, fs, TABLE_VIEW_BORDER_CHAR_MID,
                  TABLE_VIEW_BORDER_CHAR_MIDLEFT, TABLE_VIEW_BORDER_CHAR_MIDMID,
                  TABLE_VIEW_BORDER_CHAR_MIDRIGHT);
}
void s_table_view_draw_bottom_line_to_fs(
        s_table_view_data_width_t_ptr const table_view_data, FILE *const fs) {
  draw_line_to_fs(table_view_data, fs, TABLE_VIEW_BORDER_CHAR_BOTTOM,
                  TABLE_VIEW_BORDER_CHAR_BOTTOMLEFT,
                  TABLE_VIEW_BORDER_CHAR_BOTTOMMID,
                  TABLE_VIEW_BORDER_CHAR_BOTTOMRIGHT);
}

static int utf8_is_continuation(char const c) {
  return (c & 0xc0) == 0x80;
}

static size_t utf8_strlen(char const *const str) {
  size_t len = 0;
  for (size_t i = 0; str[i]; ++i)
    if (!utf8_is_continuation(str[i])) ++len;
  return len;
}

void s_table_view_draw_content_to_fs(
        s_table_view_content_t const table_view_content, FILE *fs) {
  fputs(TABLE_VIEW_BORDER_CHAR_LEFT, fs);

  for (size_t i = 0;
       i < table_view_content.table_view_data_width->column_amount; ++i) {
    fputs(table_view_content.content[i], fs);
    repeat_to_fs(fs, " ",
                 table_view_content.table_view_data_width->column_width[i]
                         - utf8_strlen(table_view_content.content[i]));
    fputs(TABLE_VIEW_BORDER_CHAR_RIGHT, fs);
  }
  fputc('\n',fs);
}
void s_table_view_draw_header_to_fs(
        s_table_view_data_width_t_ptr const table_view_data, FILE *const fs) {
  s_table_view_content_t const table_view_content = {
          .table_view_data_width = table_view_data,
          .content = table_view_data->headers};
  s_table_view_draw_content_to_fs(table_view_content, fs);
}

inline void s_table_view_draw_top_line(
        s_table_view_data_width_t_ptr const table_view_data) {
  s_table_view_draw_top_line_to_fs(table_view_data, stdout);
}
inline void s_table_view_draw_mid_line(
        s_table_view_data_width_t_ptr const table_view_data) {
  s_table_view_draw_mid_line_to_fs(table_view_data, stdout);
}
inline void s_table_view_draw_bottom_line(
        s_table_view_data_width_t_ptr const table_view_data) {
  s_table_view_draw_bottom_line_to_fs(table_view_data, stdout);
}
void s_table_view_draw_content(
        s_table_view_content_t const table_view_content) {
  s_table_view_draw_content_to_fs(table_view_content, stdout);
}
void s_table_view_draw_header(
        s_table_view_data_width_t_ptr const table_view_data) {
  s_table_view_draw_header_to_fs(table_view_data, stdout);
}

static void
draw_line_to_fs(s_table_view_data_width_t_ptr const table_view_data_width,
                FILE *const fs, char const *const main, char const *const left,
                char const *const mid, char const *const right) {
  fputs(left, fs);

  for (size_t i = 0; i < table_view_data_width->column_amount; ++i) {
    repeat_to_fs(fs, main, table_view_data_width->column_width[i]);
    if (i + 1 != table_view_data_width->column_amount) fputs(mid, fs);
  }

  fputs(right, fs);
  fputc('\n', fs);
}

static void repeat_to_fs(FILE *const fs, char const *const string,
                         size_t const amount) {
  for (size_t i = 0; i < amount; ++i) fputs(string, fs);
}
