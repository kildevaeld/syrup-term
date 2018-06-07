#pragma once

#include <stdbool.h>
#include <string.h>
#include <syrup/colors.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sy_term_style_t {
  sy_term_attr_t normal;
  sy_term_attr_t input;
  sy_term_attr_t value;
} sy_term_style_t;

sy_term_style_t default_style;

struct sy_term_form_field_cfg {
  char *msg;
  sy_term_style_t *style;
};

typedef struct sy_term_form_input_cfg {
  char *msg;
  sy_term_style_t *style;
  char *defaults;
} sy_term_form_input_cfg;

char *sy_term_form_input(sy_term_form_input_cfg *cfg);

size_t sy_term_form_list(sy_term_style_t *style, const char *msg,
                         const char **choices, size_t size);
bool sy_term_form_confirm(sy_term_style_t *style, const char *msg, bool clear);
char *sy_term_form_prompt(sy_term_style_t *style, const char *msg);
char *sy_term_form_password(sy_term_style_t *style, const char *msg,
                            const char *sub);

#ifdef __cplusplus
}
#endif