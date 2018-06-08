#include <stdarg.h>
#include <syrup/colors.h>

void sy_term_color(sy_buffer_t *buffer, sy_term_attr_t c) {

  if (c == 0)
    return;

  sy_buffer_append(buffer, (const unsigned char *)"\x1b[", 3);

  if ((c & SY_BOLD) == SY_BOLD) {
    sy_buffer_append_char(buffer, '1');
  } else if ((c & SY_UNDEDRLINE) == SY_UNDEDRLINE) {
    sy_buffer_append_char(buffer, '4');
  } else {
    sy_buffer_append_char(buffer, '0');
  }

  sy_buffer_append_char(buffer, ';');

  if ((c & SY_BACKGROUND)) {
    sy_buffer_append_char(buffer, '4');
  } else if ((c & SY_HIGHDENS) == SY_HIGHDENS) {
    sy_buffer_append_char(buffer, '9');
  } else {
    sy_buffer_append_char(buffer, '3');
  }

  if ((c & SY_BLACK) == SY_BLACK) {
    sy_buffer_append_char(buffer, '0');
  } else if ((c & SY_RED) == SY_RED) {
    sy_buffer_append_char(buffer, '1');
  } else if ((c & SY_GREEN) == SY_GREEN) {
    sy_buffer_append_char(buffer, '2');
  } else if ((c & SY_YELLOW) == SY_YELLOW) {
    sy_buffer_append_char(buffer, '3');
  } else if ((c & SY_BLUE) == SY_BLUE) {
    sy_buffer_append_char(buffer, '4');
  } else if ((c & SY_MAGENTA) == SY_MAGENTA) {
    sy_buffer_append_char(buffer, '5');
  } else if ((c & SY_CYAN) == SY_CYAN) {
    sy_buffer_append_char(buffer, '6');
  } else if ((c & SY_WHITE) == SY_WHITE) {
    sy_buffer_append_char(buffer, '7');
  }

  sy_buffer_append_char(buffer, 'm');
}

void sy_term_color_reset(sy_buffer_t *buffer) {
  sy_buffer_append(buffer, (const unsigned char *)"\x1b[0m", 5);
}

void sy_term_color_append(sy_buffer_t *buffer, sy_term_attr_t c,
                          const char *msg) {
  sy_term_color(buffer, c);
  sy_buffer_append_str(buffer, msg);
  if (c != 0)
    sy_term_color_reset(buffer);
}

void sy_term_color_appendf(sy_buffer_t *buffer, sy_term_attr_t c,
                           const char *fmt, ...) {
  sy_term_color(buffer, c);
  va_list args;
  va_start(args, fmt);
  char *out;
  if (vasprintf(&out, fmt, args) == -1) {
    return;
  }
  va_end(args);
  sy_buffer_utf8_append(buffer, out);
  free(out);
  if (c != 0)
    sy_term_color_reset(buffer);
}