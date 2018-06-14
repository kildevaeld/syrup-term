#include <stdio.h>
#include <stdlib.h>
#include <syrup/buffer.h>
#include <syrup/form.h>
#include <syrup/line-edit.h>
#include <syrup/term.h>

char *sy_term_form_input(sy_term_form_input_cfg *cfg) {

  sy_term_enable_raw_mode();

  if (!cfg->style)
    cfg->style = &default_style;

  size_t msgl = strlen(cfg->msg);

  sy_buffer_t *buf = sy_buffer_alloc();

  int row, col;
  sy_term_cursor_pos_get(&row, &col);

  sy_term_color_append(buf, cfg->style->normal, cfg->msg);

  if (cfg->defaults) {
    sy_term_color_appendf(buf, cfg->style->muted, " (%s)", cfg->defaults);
  }

  sy_buffer_append(buf, (const unsigned char *)": ", 2);

  sy_buffer_write(buf, STDOUT_FILENO);

  sy_line_edit_t edit = {
      .clear = true,
      .echo = cfg->echo,
      .row = row,
      .col = col + msgl + (cfg->defaults ? strlen(cfg->defaults) + 3 : 0) + 2,
      .style = cfg->style->input};

  char *string = NULL;
  while (true) {
    string = sy_term_line_edit_read(&edit, NULL, 0);
    if (string == NULL) {
      if (cfg->defaults)
        break;
      else if (!cfg->required)
        break;
    } else
      break;
  }

  if (!string && cfg->defaults == NULL) {
    goto end;
  } else if (!string && cfg->defaults) {
    // Okay no string - apply default
    size_t len = strlen(cfg->defaults);
    string = malloc(sizeof(char) * (len + 1));
    if (!string)
      goto end;
    memcpy(string, cfg->defaults, len);
    string[len] = '\0';
  }

  sy_term_cursor_pos_set(row, col);

  if (cfg->clear) {
    sy_buffer_clear(buf);
    sy_term_buf_erase_current_line(buf);
  } else {
    if (cfg->defaults) {
      sy_buffer_clear(buf);
      sy_term_buf_erase_current_line(buf);
      sy_term_color_appendf(buf, cfg->style->normal, "%s: ", cfg->msg);
    }

    if (cfg->echo) {
      sy_term_color_appendf(buf, cfg->style->value, "%s\n\r", string);
    } else {
      sy_buffer_append(buf, (const unsigned char *)"\n\r", 2);
    }
  }

  sy_buffer_write(buf, STDOUT_FILENO);

end:
  sy_buffer_free(buf);
  sy_term_disable_raw_mode();
  return string;
}