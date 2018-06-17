#pragma once

#include <stdbool.h>
#include <string.h>
#include <syrup/colors.h>
#include <syrup/edit.h>
#include <syrup/list-edit.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sy_term_form_field_cfg {
  SY_FORM_FIELD_FIELDS
};

typedef struct sy_term_form_input_cfg {
  SY_FORM_FIELD_FIELDS
  bool required;
  char *defaults;
  bool echo;
} sy_term_form_input_cfg;

typedef struct sy_term_form_confirm_cfg {
  SY_FORM_FIELD_FIELDS
  bool required;
  bool defaults;
} sy_term_form_confirm_cfg;

typedef struct sy_term_form_select_cfg {
  SY_FORM_FIELD_FIELDS
  int *defaults;
  int defaults_len;
  int min_selected;
  int max_selected;
  char *selected;
  char *unselected;
  int max_height;
} sy_term_form_select_cfg;

char *sy_term_form_input(sy_term_form_input_cfg *cfg);
bool sy_term_form_confirm(sy_term_form_confirm_cfg *cfg);
sy_list_edit_res_t *sy_term_form_select(sy_term_form_select_cfg *cfg,
                                        char **choices, size_t len);

#ifdef __cplusplus
}
#endif