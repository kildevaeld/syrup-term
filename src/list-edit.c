#include <syrup/array-list.h>
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
                       int index, sy_array_t *selected) {
  sy_buffer_t *buf = sy_buffer_alloc();

  int row = le->row;

  sy_term_cursor_buf_pos_set(buf, row, le->col);
  for (int i = 0; i < len; i++) {
    sy_term_cursor_buf_pos_set(buf, row + i, le->col);
    sy_term_buf_erase_line(buf);
    if (le->max_select > 1) {
      sy_buffer_append_char(buf, sy_array_indexof(selected, i) != -1
                                     ? le->selected
                                     : le->unselected);
    }
    if (i == index) {
      sy_term_color_appendf(buf, le->highlight, "%s\n", choices[i]);
    } else {
      sy_buffer_utf8_appendf(buf, "%s\n", choices[i]);
    }
  }
  sy_term_cursor_buf_pos_set(buf, row + index, le->col);
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

static bool has_value(int *values, int len, int val) {
  for (int i = 0; i < len; i++) {
    if (values[i] == val)
      return true;
  }
  return false;
}

static bool remove_value(int *values, int len, int val) {
  for (int i = 0; i < len; i++) {
    if (values[i] == val) {
      if (i == 0) {
        memcpy(values, values + 1, len - 1);
      } else {
        // memcpy()
      }
      // memcpy(values, )
    }
  }
  return false;
}

static int compare(void *lh, void *rh) {
  int l = (int)lh;
  int r = (int)rh;
  // printf("compare %i %i\n", l, r);
  if (l > r)
    return -1;
  else if (l < r)
    return 1;
  return 0;
}

size_t sy_term_list_edit_read(sy_list_edit_t *le, char **choices, size_t len) {
  sy_buffer_t *buffer = sy_buffer_alloc();

  sy_term_enable_raw_mode();
  sy_term_cursor_hide();

  int index = 0;
  int max_height = le->max_height ? le->max_height : 5;

  size_t l = len;

  int offset = 0;

  int rows, cols, or ;
  sy_term_size(&rows, &cols);
  or = le->row;

  int orow, ocol;
  sy_term_cursor_pos_get(&orow, &ocol);
  int bh = min(max_height, len);

  sy_array_t *sel_idx = sy_array_new(compare);

  print_list(le, choices, bh, index, sel_idx);

  if (bh + le->row > rows) {
    le->row -= (bh + le->row) - rows;
  }

  int selected_indexes[le->max_select ? le->max_select : 1];
  int selections = 0;

  while (1) {
    int key = sy_term_read_key();
    switch (key) {
    case SY_ARROW_DOWN: {
      if (index == bh - 1) {
        if (len > bh && offset + bh < len) {
          offset++;
          index--;
        } else if (offset + bh + len) {
          offset = 0;
          index = -1;
        } else {
          break;
        }
      }
      print_list(le, choices + offset, bh, ++index, sel_idx);
    } break;
    case SY_ARROW_UP:
      if (index == 0) {
        if (len > bh && offset > 0) {
          offset--;
          index++;
        } else if (offset == 0 && len > bh) {
          offset = len - bh;
          index = bh;
        } else {
          break;
        }
      }
      print_list(le, choices + offset, bh, --index, sel_idx);
      break;
    case SY_ENTER_KEY:
      if (le->max_select > 1 && selections < le->min_select) {
        break;
      }
      goto end;
    case ' ': {
      if (le->max_select < 2) {

        break;
      }

      size_t idx = sy_array_indexof(sel_idx, index + offset);
      if (idx != -1)
        sy_array_remove_index(sel_idx, idx);
      else
        sy_array_append(sel_idx, index + offset);

      // sy_array_indexof(sel_idx, index + offset);
      print_list(le, choices + offset, bh, index, sel_idx);
    }
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
    sy_buffer_append_str(buffer, choices[index + offset]);
  }

  sy_buffer_write(buffer, STDOUT_FILENO);
  sy_term_cursor_show();

  return index + offset;
}