#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <syrup/colors.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sy_line_edit_t {
  bool echo;
  bool clear;
  bool return_on_maxlen;
  sy_term_attr_t style;
  int row;
  int col;
} sy_line_edit_t;

void sy_term_line_edit_init(sy_line_edit_t *le);
char *sy_term_line_edit_read(sy_line_edit_t *le, char *out, size_t maxlen);

#ifdef __cplusplus
}
#endif