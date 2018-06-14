#include "private.h"
#include <stdio.h>
#include <stdlib.h>
#include <syrup/buffer.h>
#include <syrup/form.h>
#include <syrup/list-edit.h>
#include <syrup/term.h>

sy_list_edit_res_t *sy_term_form_select(sy_term_form_select_cfg *cfg,
                                        char **choices, size_t len) {
  sy_term_enable_raw_mode();

  if (!cfg->style)
    cfg->style = &default_style;

  size_t msgl = strlen(cfg->msg);

  sy_buffer_t *buf = sy_buffer_alloc();

  int row, col;
  sy_term_cursor_pos_get(&row, &col);
  sy_term_color_append(buf, cfg->style->normal, cfg->msg);
  sy_buffer_append(buf, (const unsigned char *)"\n\r", 2);

  sy_buffer_write(buf, STDOUT_FILENO);

  sy_list_edit_t ecfg = {
    .col = cfg->col,
    .row = cfg->row,
    .selected = cfg->selected,
    .unselected = cfg->unselected
  };
}