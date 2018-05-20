#pragma once
#include <syrup/buffer.h>

enum sy_term_colors {
  SY_BLACK = 1 << 0,
  SY_RED = 1 << 2,
  SY_GREEN = 1 << 3,
  SY_YELLOW = 1 << 4,
  SY_BLUE = 1 << 5,
  SY_PURPLE = 1 << 6,
  SY_CYAN = 1 << 7,
  SY_WHITE = 1 << 8,

  SY_BOLD = 1 << 9,
  SY_UNDEDRLINE = 1 << 10,
  SY_BACKGROUND = 1 << 11,
  SY_HIGHDENS = 1 << 12
};

void sy_term_color(sy_buffer_t *buffer, enum sy_term_colors c);
void sy_term_color_reset(sy_buffer_t *buffer);