#include <syrup/list-edit.h>
#include <syrup/term.h>

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

static void print_list(sy_list_edit_t *le, char **choices, size_t len,
                       int index, bool first) {
  sy_buffer_t *buf = sy_buffer_alloc();

  int row = le->row; // first ? le->row : le->row - len;

  sy_term_cursor_buf_pos_set(buf, row, le->col);
  for (int i = 0; i < len; i++) {
    sy_term_cursor_buf_pos_set(buf, row + i, le->col);
    if (i == index) {
      sy_term_color_appendf(buf, le->highlight, "%s\n", choices[i]);
    } else {
      sy_buffer_utf8_appendf(buf, "%s\n", choices[i]);
    }
  }
  sy_term_cursor_buf_pos_set(buf, (first ? row - len : row) + index, le->col);
  sy_buffer_write(buf, STDOUT_FILENO);
  sy_buffer_free(buf);
}

void sy_term_list_edit_init(sy_list_edit_t *le) {
  memset(le, 0, sizeof(sy_list_edit_t));
  int row, col;
  sy_term_enable_raw_mode();
  sy_term_cursor_pos_get(&row, &col);
  sy_term_disable_raw_mode();
  le->row = row;
  le->col = col;
}

size_t sy_term_list_edit_read(sy_list_edit_t *le, char **choices, size_t len) {
  sy_buffer_t *buffer = sy_buffer_alloc();

  sy_term_enable_raw_mode();

  sy_term_cursor_hide();
  int index = 0;
  int max_height = le->max_height ? le->max_height : 10;

  size_t l = len;

  int rows, cols, or ;
  sy_term_size(&rows, &cols);
  or = le->row;

  int orow, ocol;
  sy_term_cursor_pos_get(&orow, &ocol);

  print_list(le, choices, len, index, true);

  int bh = min(max_height, len);
  if (bh + le->row > rows) {
    le->row -= (bh + le->row) - rows;
  }

  while (1) {
    int key = sy_term_read_key();
    switch (key) {
    case SY_ARROW_DOWN:
      if (index == len - 1)
        break;
      print_list(le, choices, len, ++index, false);
      break;
    case SY_ARROW_UP:
      if (index == 0)
        break;
      print_list(le, choices, len, --index, false);
      break;
    case SY_ENTER_KEY:
      goto end;
    }
  }

end:

  sy_buffer_clear(buffer);
  sy_term_cursor_buf_pos_set(buffer, le->row + bh, le->col);
  while (bh) {
    sy_term_buf_erase_current_line(buffer);
    sy_term_cursor_buf_pos_set(buffer, le->row + (--bh), le->col);
  }

  if (!le->clear) {
    sy_buffer_append_str(buffer, choices[index]);
  }

  sy_buffer_write(buffer, STDOUT_FILENO);
  sy_term_cursor_show();

  return index;
}