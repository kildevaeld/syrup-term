#include <stdio.h>
#include <syrup/buffer.h>
#include <syrup/form.h>
#include <syrup/term.h>

void print_input(sy_term_style_t *style, sy_buffer_t *str, int row, int col) {
  sy_term_erase_current_line();
  sy_term_cursor_tostart(str);
  /*char buf[sy_buffer_len(str) + 1];
  buf[0] = '\r';
  sy_buffer_copy(str, buf + 1);
  write(STDOUT_FILENO, buf, sy_buffer_len(str) + 1);*/

  sy_buffer_write(str, STDOUT_FILENO);
  sy_term_cursor_pos_set(row, col);
}

static int print_line(const char *msg, bool highlight) {
  int len = strlen(msg) + 20;
  char buf[len];
  if (!highlight)
    snprintf(buf, len, "%s\r", msg);
  else
    snprintf(buf, len, "\x1b[7m%s\x1b[0m\r", msg);
  write(STDOUT_FILENO, buf, strlen(buf));

  return len;
}

size_t sy_term_form_list(sy_term_style_t *style, const char *msg,
                         const char **choices, size_t len) {
  sy_term_enable_raw_mode();

  char buf[strlen(msg) + 3];
  snprintf(buf, strlen(msg) + 3, "%s\r\n", msg);
  write(STDOUT_FILENO, buf, strlen(buf));
  for (int i = 0; i < len; i++) {
    print_line(choices[i], i == 0);
    write(STDOUT_FILENO, "\n", 1);
  }
  sy_term_cursor_up(len);
  int i = 0;
  while (1) {
    int c = sy_term_read_key();
    switch (c) {
    case SY_CTRL_KEY('c'):
      i = -1;
      goto end;
    case SY_ARROW_DOWN:
      if (i >= len - 1)
        break;
      sy_term_erase_line();
      print_line(choices[i], 0);

      sy_term_cursor_down(1);
      print_line(choices[++i], 1);
      break;
    case SY_ARROW_UP:
      if (i == 0)
        break;
      sy_term_erase_line();
      print_line(choices[i], 0);

      sy_term_cursor_up(1);
      print_line(choices[--i], 1);

      break;
    case SY_ENTER_KEY:
      goto end;
    }
  }

end:

  do {
    int diff = len - i;
    int l = len;
    sy_term_cursor_down(diff);
    while (len--) {
      sy_term_erase_line();
      sy_term_cursor_up(1);
    }
  } while (0);

  sy_term_cursor_up(1);
  int slen = strlen(msg) + 1 + strlen(choices[i]) + 4;
  char sbuf[slen];
  snprintf(sbuf, slen, "%s %s\r\n", msg, choices[i]);
  write(STDOUT_FILENO, sbuf, strlen(sbuf));
  sy_term_disable_raw_mode();
  return i;
}

bool sy_term_form_confirm(sy_term_style_t *style, const char *msg, bool clear) {
  sy_term_enable_raw_mode();

  char buf[strlen(msg) + 3];
  snprintf(buf, strlen(msg) + 3, "%s ", msg);
  write(STDOUT_FILENO, buf, strlen(buf));
  bool ret = false;
  char *result = NULL;
  while (1) {
    int c = sy_term_read_key();

    switch (c) {
    case 'j':
    case 'y':
      ret = true;
      result = "yes\r\n";
      goto end;
    case 'n':
      result = "no\r\n";
      goto end;
    }
  }

end:

  if (clear) {
    sy_term_erase_current_line();
    write(STDOUT_FILENO, "\r", 1);
  } else {
    write(STDOUT_FILENO, result, strlen(result));
  }

  sy_term_disable_raw_mode();

  return ret;
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

  print_input(style, str, row, msg_l);

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
      print_input(style, str, row, cur - 1);

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

      print_input(style, str, row, cur);
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
                            const char *sub) {
  sy_term_enable_raw_mode();

  size_t msg_l = strlen(msg) + 2;

  sy_buffer_t *str = sy_buffer_alloc();
  sy_buffer_utf8_appendf(str, "%s ", msg);

  int row, col;
  sy_term_cursor_pos_get(&row, &col);

  print_input(style, str, row, msg_l - 1);

  int idx = msg_l;
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

      // print_input(str, row, cur);
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