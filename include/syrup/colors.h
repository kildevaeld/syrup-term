#pragma once
#include <syrup/buffer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum sy_term_attr_t {
  SY_BLACK = 1 << 0,
  SY_RED = 1 << 1,
  SY_GREEN = 1 << 2,
  SY_YELLOW = 1 << 3,
  SY_BLUE = 1 << 4,
  SY_MAGENTA = 1 << 5,
  SY_CYAN = 1 << 6,
  SY_WHITE = 1 << 7,

  SY_BOLD = 1 << 8,
  SY_UNDEDRLINE = 1 << 9,
  SY_BACKGROUND = 1 << 10,
  SY_HIGHDENS = 1 << 11
} sy_term_attr_t;

void sy_term_color(sy_buffer_t *buffer, sy_term_attr_t c);

void sy_term_color_append(sy_buffer_t *buffer, sy_term_attr_t c,
                          const char *msg);

void sy_term_color_appendf(sy_buffer_t *buffer, sy_term_attr_t c,
                           const char *msg, ...);

void sy_term_color_reset(sy_buffer_t *buffer);

#ifdef __cplusplus
}
#endif