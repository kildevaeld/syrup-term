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
  sy_term_cursor_buf_pos_set(buf, cfg->row, cfg->col);
  sy_term_color_append(buf, cfg->style->normal, cfg->msg);
  sy_buffer_append(buf, (const unsigned char *)":\n", 2);

  sy_buffer_write(buf, STDOUT_FILENO);

  sy_list_edit_t ecfg = {.col = cfg->col,
                         .row = cfg->row + 1,
                         .selected = cfg->selected ? cfg->selected : "1",
                         .unselected = cfg->unselected ? cfg->unselected : "0",
                         .max_select = cfg->max_selected,
                         .min_select = cfg->min_selected,
                         .highlight = cfg->style->highlight
                                          ? cfg->style->highlight
                                          : SY_BOLD | SY_HIGHDENS,
                         .max_height = cfg->max_height};

  sy_list_edit_res_t *res = sy_term_list_edit_read(&ecfg, choices, len);
  if (!res) {
    goto end;
  }

  sy_term_cursor_pos_set(ecfg.row - 1, cfg->col);

  if (cfg->clear) {
    sy_buffer_clear(buf);
    sy_term_buf_erase_current_line(buf);
  } else {
    sy_buffer_clear(buf);
    sy_term_color_appendf(buf, cfg->style->normal, "%s: ", cfg->msg);

    if (cfg->style->value) {
      sy_term_color(buf, cfg->style->value);
    }
    for (int i = 0; i < res->len; i++) {
      sy_buffer_append_str(buf, choices[i]);
      if (i != res->len - 1) {
        sy_buffer_append(buf, (const unsigned char *)", ", 2);
      }
    }
    if (cfg->style->value) {
      sy_term_color_reset(buf);
    }
    sy_buffer_append(buf, (const unsigned char *)"\n\r", 2);
  }

  sy_buffer_write(buf, STDOUT_FILENO);

end:
  sy_buffer_free(buf);
  sy_term_disable_raw_mode();

  return res;
}