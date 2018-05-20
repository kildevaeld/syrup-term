#include <syrup/buffer.h>
#include <syrup/form.h>
#include <syrup/term.h>

void print_input(sy_buffer_t *str, int row, int col) {
  sy_term_erase_current_line();
  sy_term_cursor_tostart(str);
  /*char buf[cs_str_len(str) + 1];
  buf[0] = '\r';
  cs_str_copy(str, buf + 1);
  write(STDOUT_FILENO, buf, cs_str_len(str) + 1);*/

  sy_buffer_write(str, STDOUT_FILENO);
  sy_term_cursor_pos_set(row, col);
}

size_t sy_term_form_list(sy_term_style_t *style, const char *msg,
                         const char **choices, size_t size) {}

bool sy_term_form_confirm(sy_term_style_t *style, const char *msg, bool clear) {
}

/*static void read_n(char *buf, int c) {
  int i = 0;
  while (i < c) {
    buf[i++] = sy_term_read_key();
  }
}*/

static int utf_width(char c) {
  if (SY_IS_UTF8_2C(c))
    return 2;
  else if (SY_IS_UTF8_3C(c))
    return 3;
  else if (SY_IS_UTF8_4C(c))
    return 4;
  return 1;
}

char *sy_term_form_prompt(sy_term_style_t *style, const char *msg) {
  sy_term_enable_raw_mode();

  size_t msg_l = strlen(msg) + 1;

  sy_buffer_t *str = sy_buffer_alloc();
  sy_buffer_utf8_appendf(str, "%s ", msg);

  int row, col;
  sy_term_cursor_pos_get(&row, &col);

  print_input(str, row, msg_l);

  int idx = msg_l + 1;
  int cur = idx;
  while (1) {
    int key = sy_term_read_key();

    switch (key) {
    case SY_CTRL_KEY('c'):
      goto end;
    case SY_ARROW_LEFT:
      if (idx == cur)
        break;
      cur--;
      sy_term_cursor_backward(1);
      break;
    case SY_ARROW_RIGHT:
      if (cur > sy_buffer_utf8_len(str))
        break;
      cur++;
      sy_term_cursor_forward(1);
      break;
    case SY_BACKSPACE:
      if (cur <= idx)
        break;
      cur--;

      sy_buffer_utf8_remove(str, cur - 1, 1);
      print_input(str, row, cur - 1);

      break;
    case SY_ENTER_KEY:
      write(STDOUT_FILENO, "\n\r", 2);
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
        sy_buffer_utf8_insert(str, cur - 1, buf);

      } else {
        sy_buffer_utf8_insert_char(str, cur - 1, key);
      }

      print_input(str, row, cur);
      cur++;
    }
    }
  }

end:
  sy_term_disable_raw_mode();
  sy_buffer_remove(str, 0, msg_l - 1);
  char *out = sy_buffer_string(str);
  sy_buffer_free(str);
  return out;
}

char *sy_term_form_password(sy_term_style_t *style, const char *msg,
                            const char *sub) {}