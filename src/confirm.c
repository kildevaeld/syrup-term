#include <stdio.h>
#include <stdlib.h>
#include <syrup/buffer.h>
#include <syrup/form.h>
#include <syrup/term.h>

char *sy_term_form_confirm(sy_term_form_confirm_cfg *cfg) {
  sy_term_enable_raw_mode();

  if (!cfg->style)
    cfg->style = &default_style;

  size_t msgl = strlen(cfg->msg);

  sy_buffer_t *buf = sy_buffer_alloc();

  int row, col;
  sy_term_cursor_pos_get(&row, &col);

  sy_term_color_append(buf, cfg->style->normal, cfg->msg);

  sy_buffer_append(buf, "[y/n]: ", 8);

  sy_buffer_write(buf, STDOUT_FILENO);

  bool ret = false;

  while (1) {
    int c = sy_term_read_key();

    switch (c) {
    case 'j':
    case 'y':
      ret = true;
    case 'n':
      goto end;
    }
  }

end:
  sy_buffer_free(buf);

  return ret;
}