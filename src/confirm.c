#include "private.h"
#include <stdio.h>
#include <stdlib.h>
#include <syrup/buffer.h>
#include <syrup/form.h>
#include <syrup/term.h>

bool sy_term_form_confirm(sy_term_form_confirm_cfg *cfg) {
  sy_term_enable_raw_mode();

  if (!cfg->style)
    cfg->style = &default_style;

  size_t msgl = strlen(cfg->msg);

  sy_buffer_t *buf = sy_buffer_alloc();

  int row, col;
  sy_term_cursor_pos_get(&row, &col);

  if (row != cfg->row || col != cfg->col) {
    sy_term_cursor_buf_pos_set(buf, cfg->row, cfg->col);
  }

  sy_term_color_append(buf, cfg->style->normal, cfg->msg);

  sy_buffer_append(buf, (const unsigned char *)"[y/n]: ", 8);

  sy_buffer_write(buf, STDOUT_FILENO);

  bool ret = false;

  while (1) {
    int c = sy_term_read_key();

    handle_terminal_signals(c);

    switch (c) {
    case 'j':
    case 'y':
      ret = true;
    case 'n':
      goto end;
    }
  }

end:

  sy_buffer_clear(buf);
  sy_term_cursor_buf_pos_set(buf, cfg->row, cfg->col);
  sy_term_buf_erase_current_line(buf);
  if (!cfg->clear) {
    sy_term_color_appendf(buf, cfg->style->normal, "%s: ", cfg->msg);
    sy_term_color_appendf(buf, cfg->style->value, "%s\n\r", ret ? "yes" : "no");
  }
  sy_buffer_write(buf, STDOUT_FILENO);
  sy_term_disable_raw_mode();
  sy_buffer_free(buf);

  return ret;
}