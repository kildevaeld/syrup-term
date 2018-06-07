#include <stdio.h>
#include <syrup/buffer.h>
#include <syrup/form.h>
#include <syrup/term.h>

static void print_input2(sy_buffer_t *buf, struct sy_term_form_field_cfg *cfg,
                         sy_buffer_t *value, int row, int col) {

  sy_buffer_clear(buf);
  sy_term_buf_erase_current_line(buf);
  sy_term_cursor_buf_tostart(buf);
  if (cfg->style->normal)
    sy_term_color(buf, cfg->style->normal);
  sy_buffer_utf8_append(buf, cfg->msg);
  sy_buffer_append_char(buf, ' ');
  if (cfg->style->normal)
    sy_term_color_reset(buf);
  if (cfg->style->input)
    sy_term_color(buf, cfg->style->input);
  sy_buffer_append_buffer(buf, value);
  if (cfg->style->input)
    sy_term_color_reset(buf);
  sy_term_cursor_buf_pos_set(
      buf, row, strlen(cfg->msg) + sy_buffer_utf8_len(value) + 1 + col);
  sy_buffer_write(buf, STDOUT_FILENO);
}

char *sy_term_form_input(sy_term_form_input_cfg *cfg) {

  if (!cfg->style)
    cfg->style = &default_style;

  sy_buffer_t *output = sy_buffer_alloc();
  sy_buffer_t *buffer = sy_buffer_alloc();

  int row, col;
  sy_term_cursor_pos_get(&row, &col);

  print_input2(buffer, cfg, output, row, 0);

end:
  sy_buffer_free(output);
  sy_buffer_free(buffer);
}