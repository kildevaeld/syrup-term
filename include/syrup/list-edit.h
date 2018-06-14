#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <syrup/colors.h>
#include <syrup/edit.h>

typedef struct sy_list_edit_t {
  SY_EDITOR_FIELDS
  int max_height;
  sy_term_attr_t highlight;
  int max_select;
  int min_select;
  char *selected;
  char *unselected;

} sy_list_edit_t;

typedef struct sy_list_edit_res_t {
  int len;
  int *indexes;
} sy_list_edit_res_t;

void sy_term_list_edit_init(sy_list_edit_t *le);
sy_list_edit_res_t *sy_term_list_edit_read(sy_list_edit_t *le, char **choices,
                                           size_t len);
void sy_term_list_res_free(sy_list_edit_res_t *res);