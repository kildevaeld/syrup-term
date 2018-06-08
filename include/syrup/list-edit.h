#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <syrup/colors.h>

typedef struct sy_list_edit_t {
  bool clear;
  int row;
  int col;
  int max_height;
  sy_term_attr_t highlight;
} sy_list_edit_t;

void sy_term_list_edit_init(sy_list_edit_t *le);
size_t sy_term_list_edit_read(sy_list_edit_t *le, char **choices, size_t len);