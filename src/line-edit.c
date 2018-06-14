#include "private.h"
#include <string.h>
#include <syrup/line-edit.h>
#include <syrup/term.h>

void sy_term_line_edit_init(sy_line_edit_t *le) {
  memset(le, 0, sizeof(sy_line_edit_t));
  int row, col;
  sy_term_enable_raw_mode();
  sy_term_cursor_pos_get(&row, &col);
  sy_term_disable_raw_mode();
  le->row = row;
  le->col = col;
  le->echo = true;
}

static int utf_width(char c) {
  if (SY_IS_UTF8_2C(c))
    return 2;
  else if (SY_IS_UTF8_3C(c))
    return 3;
  else if (SY_IS_UTF8_4C(c))
    return 4;
  return 1;
}

static void print_line_edit(sy_line_edit_t *le, sy_buffer_t *buffer,
                            int index) {
  sy_buffer_t *buf = sy_buffer_alloc();
  sy_term_cursor_buf_pos_set(buf, le->row, le->col);
  sy_term_buf_erase_line(buf);
  if (le->style)
    sy_term_color(buf, le->style);
  sy_buffer_append_buffer(buf, buffer);
  if (le->style)
    sy_term_color_reset(buf);
  sy_term_cursor_buf_pos_set(buf, le->row, le->col + index);
  sy_buffer_write(buf, STDOUT_FILENO);
}

char *sy_term_line_edit_read(sy_line_edit_t *le, char *out, size_t maxlen) {
  sy_buffer_t *buffer = sy_buffer_alloc();
  int orow, ocol;
  sy_term_enable_raw_mode();
  sy_term_cursor_pos_get(&orow, &ocol);
  sy_term_cursor_pos_set(le->row, le->col);
  int index = 0;

  while (true) {
    int key = sy_term_read_key();

    handle_terminal_signals(key);

    switch (key) {
    case SY_CTRL_KEY('c'):
      goto end;
    case SY_ARROW_LEFT:
      if (index == 0)
        break;
      index--;
      sy_term_cursor_backward(1);
      break;
    case SY_ARROW_RIGHT:
      if (index >= sy_buffer_utf8_len(buffer))
        break;
      index++;
      sy_term_cursor_forward(1);
      break;
    case SY_BACKSPACE:
      if (index <= 0)
        break;
      index--;
      sy_buffer_utf8_remove(buffer, index, 1);
      if (le->echo)
        print_line_edit(le, buffer, index);
      break;
    case SY_ENTER_KEY:
      goto end;
    case SY_ARROW_DOWN:
    case SY_ARROW_UP:
      break;
    default: {

      if (SY_IS_UTF(key)) {
        int ul = utf_width(key);

        char buf[ul + 1];
        buf[0] = key;
        sy_term_nread_key(buf + 1, ul - 1);
        buf[ul] = '\0';

        if (out && (ul + sy_buffer_len(buffer) > maxlen)) {
          if (le->return_on_maxlen) {
            goto end;
          }
          break;
          //
        }

        sy_buffer_utf8_insert(buffer, index, buf);

      } else {
        if (out && (1 + sy_buffer_len(buffer) > maxlen)) {
          if (le->return_on_maxlen) {
            goto end;
          }
          break;
          // goto end;
        }
        sy_buffer_utf8_insert_char(buffer, index, key);
      }
      index++;
      if (le->echo)
        print_line_edit(le, buffer, index);
    }
    }
  }

end:
  if (le->clear) {
    sy_term_cursor_pos_set(le->row, le->col);
    sy_term_erase_line();
  }
  sy_term_cursor_pos_set(orow, ocol);
  sy_term_disable_raw_mode();

  if (sy_buffer_len(buffer) == 0) {
    sy_buffer_free(buffer);
    return NULL;
  }

  if (out) {
    sy_buffer_copy(buffer, (unsigned char *)out);
    out[sy_buffer_len(buffer)] = '\0';
    sy_buffer_free(buffer);
    return out;
  } else {
    char *str = sy_buffer_string(buffer);
    sy_buffer_free(buffer);
    return str;
  }
  return NULL;
}