#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sy_term_style_t {
  sy_term_attr_t normal;
  sy_term_attr_t input;
  sy_term_attr_t value;
  sy_term_attr_t muted;
} sy_term_style_t;

sy_term_style_t default_style;

#define SY_EDITOR_FIELDS                                                       \
  int row;                                                                     \
  int col;

#define SY_FORM_FIELD_FIELDS                                                   \
  int row;                                                                     \
  int col;                                                                     \
  char *msg;                                                                   \
  sy_term_style_t *style;                                                      \
  bool clear;

#ifdef __cplusplus
}
#endif