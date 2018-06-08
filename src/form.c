#include <stdio.h>
#include <syrup/buffer.h>
#include <syrup/form.h>
#include <syrup/term.h>

static void print_input(sy_buffer_t *buf, sy_term_style_t *style, const char *m,
                        sy_buffer_t *str, int row, int col) {

  sy_buffer_clear(buf);
  // sy_buffer_t *buf = sy_buffer_alloc();
  sy_term_buf_erase_current_line(buf);
  sy_term_cursor_buf_tostart(buf);
  if (style->normal)
    sy_term_color(buf, style->normal);
  sy_buffer_utf8_append(buf, m);
  sy_buffer_append_char(buf, ' ');
  if (style->normal)
    sy_term_color_reset(buf);
  if (style->input)
    sy_term_color(buf, style->input);
  sy_buffer_append_buffer(buf, str);
  if (style->input)
    sy_term_color_reset(buf);
  sy_term_cursor_buf_pos_set(buf, row,
                             strlen(m) + sy_buffer_utf8_len(str) + 1 + col);
  sy_buffer_write(buf, STDOUT_FILENO);
}

static void print_result(sy_buffer_t *buf, sy_term_style_t *style,
                         const char *m, sy_buffer_t *str) {
  sy_buffer_clear(buf);
  // sy_buffer_t *buf = sy_buffer_alloc();
  sy_term_buf_erase_current_line(buf);
  sy_term_cursor_buf_tostart(buf);
  if (style->normal)
    sy_term_color(buf, style->normal);
  sy_buffer_utf8_append(buf, m);
  sy_buffer_append_char(buf, ' ');
  if (style->normal)
    sy_term_color_reset(buf);
  if (style->value)
    sy_term_color(buf, style->value);
  sy_buffer_append_buffer(buf, str);
  if (style->value)
    sy_term_color_reset(buf);
  sy_buffer_append(buf, "\n\r", 2);

  sy_buffer_write(buf, STDOUT_FILENO);
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

/*bool sy_term_form_confirm(sy_term_style_t *style, const char *msg, bool clear)
{ sy_term_enable_raw_mode();

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
  sy_buffer_t *buffer = sy_buffer_alloc();

  int row, col;
  sy_term_cursor_pos_get(&row, &col);

  print_input(buffer, style, msg, str, row, 0);

  int index = 0;
  while (1) {
    int key = sy_term_read_key();

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
      if (index > sy_buffer_utf8_len(str))
        break;
      index++;
      sy_term_cursor_forward(1);
      break;
    case SY_BACKSPACE:
      if (index <= 0)
        break;
      index--;
      sy_buffer_utf8_remove(str, index, 1);
      print_input(buffer, style, msg, str, row, 0);
      break;
    case SY_ENTER_KEY:
      // write(STDOUT_FILENO, "\n\r", 2);
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
        sy_buffer_utf8_insert(str, index, buf);

      } else {
        sy_buffer_utf8_insert_char(str, index, key);
      }
      print_input(buffer, style, msg, str, row, 0);
      index++;
    }
    }
  }

end:
  print_result(buffer, style, msg, str);
  sy_term_disable_raw_mode();
  char *out = sy_buffer_string(str);
  sy_buffer_free(str);
  sy_buffer_free(buffer);
  return out;
}

char *sy_term_form_password(sy_term_style_t *style, const char *msg,
                            const char *sub) {
  sy_term_enable_raw_mode();

  size_t msg_l = strlen(msg) + 1;

  sy_buffer_t *str = sy_buffer_alloc();
  sy_buffer_t *buffer = sy_buffer_alloc();
  int row, col;
  sy_term_cursor_pos_get(&row, &col);

  print_input(buffer, style, msg, str, row, 0);

  int index = 0;
  while (1) {
    int key = sy_term_read_key();

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
      if (index > sy_buffer_utf8_len(str))
        break;
      index++;
      sy_term_cursor_forward(1);
      break;
    case SY_BACKSPACE:
      if (index <= 0)
        break;
      index--;
      sy_buffer_utf8_remove(str, index, 1);
      // print_input(style, msg, str, row, 0);
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
        sy_buffer_utf8_insert(str, index, buf);

      } else {
        sy_buffer_utf8_insert_char(str, index, key);
      }
      // print_input(style, msg, str, row, 0);
      index++;
    }
    }
  }

end:
  sy_term_disable_raw_mode();
  char *out = sy_buffer_string(str);
  sy_buffer_free(str);
  sy_buffer_free(buffer);
  return out;
}