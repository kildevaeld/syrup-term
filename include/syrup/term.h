#pragma once

#include <stdbool.h>
#include <syrup/buffer.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SY_CTRL_KEY(k) ((k)&0x1f)

enum type_term_control_keys_t {
  SY_ENTER_KEY = 13,
  SY_BACKSPACE = 127,
  SY_ARROW_LEFT = 1000,
  SY_ARROW_RIGHT,
  SY_ARROW_UP,
  SY_ARROW_DOWN,
  SY_DEL_KEY,
  SY_HOME_KEY,
  SY_END_KEY,
  SY_PAGE_UP,
  SY_PAGE_DOWN,

  SY_UNICODE_4C,
  SY_UNICODE_3C,
  SY_UNICODE_2C
};

bool sy_term_enable_raw_mode();
bool sy_term_disable_raw_mode();

void sy_term_clear_screen();
int sy_term_read_key();
void sy_term_nread_key(char *buf, int c);
int sy_term_cursor_pos_get(int *row, int *col);
int sy_term_size(int *rows, int *cols);

void sy_term_cursor_pos_set(int row, int col);
void sy_term_cursor_up(int n);
void sy_term_cursor_down(int n);
void sy_term_cursor_forward(int n);
void sy_term_cursor_backward(int n);
void sy_term_cursor_tostart();

void sy_term_erase_line();
void sy_term_erase_current_line();
void sy_term_cursor_show();
void sy_term_cursor_hide();

void sy_term_clear_screen_buf(sy_buffer_t *buffer);
void sy_term_cursor_buf_pos_set(sy_buffer_t *buffer, int row, int col);
void sy_term_cursor_buf_up(sy_buffer_t *buffer, int n);
void sy_term_cursor_buf_down(sy_buffer_t *buffer, int n);
void sy_term_cursor_buf_forward(sy_buffer_t *buffer, int n);
void sy_term_cursor_buf_backward(sy_buffer_t *buffer, int n);
void sy_term_buf_erase_line(sy_buffer_t *buffer);
void sy_term_buf_erase_current_line(sy_buffer_t *buffer);
void sy_term_cursor_buf_tostart(sy_buffer_t *buffer);

void sy_term_cursor_buf_show(sy_buffer_t *buffer);
void sy_term_cursor_buf_hide(sy_buffer_t *buffer);

#ifdef __cplusplus
}
#endif