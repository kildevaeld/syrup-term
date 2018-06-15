#include "private.h"
#include <string.h>
#include <syrup/array-list.h>
#include <syrup/list-edit.h>
#include <syrup/term.h>

static void print_list(sy_list_edit_t *le, char **choices, size_t len,
                       int index, sy_array_t *selected, int offset) {
  sy_buffer_t *buf = sy_buffer_alloc();

  int row = le->row;

  sy_term_cursor_buf_pos_set(buf, row, le->col);
  for (int i = 0; i < len; i++) {
    sy_term_cursor_buf_pos_set(buf, row + i, le->col);
    sy_term_buf_erase_line(buf);
    if (le->max_select > 1) {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-conversion"
      sy_term_color_append(buf, i == index ? le->highlight : 0,
                           sy_array_indexof(selected, (i + offset)) != -1
                               ? le->selected
                               : le->unselected);
#pragma clang diagnostic pop
      sy_buffer_append_char(buf, ' ');
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

static int compare(const void *lh, const void *rh) {
  int l = (int)lh;
  int r = (int)rh;
  if (l < r)
    return -1;
  else if (l > r)
    return 1;
  return 0;
}

static sy_list_edit_res_t *return_result(sy_array_t *a) {
  int *i = malloc(sizeof(int) * sy_array_len(a));
  sy_list_edit_res_t *res = NULL;
  if (!i) {
    goto end;
  }
  for (int idx = 0; idx < sy_array_len(a); idx++) {
    i[idx] = (int)sy_array_get(a, idx);
  }
  res = malloc(sizeof(sy_list_edit_res_t));
  if (!res) {
    free(i);
    goto end;
  }
  res->len = sy_array_len(a);
  res->indexes = i;

end:
  sy_array_free(a);
  return res;
}

sy_list_edit_res_t *sy_term_list_edit_read(sy_list_edit_t *le, char **choices,
                                           size_t len) {
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
  int bh = sy_min(max_height, len);

  sy_array_t *sel_idx = sy_array_new(compare);

  print_list(le, choices, bh, index, sel_idx, offset);

  if (bh + le->row > rows) {
    le->row -= (bh + le->row) - rows;
  }

  int selected_indexes[le->max_select ? le->max_select : 1];
  int selections = 0;

  while (1) {
    int key = sy_term_read_key();

    handle_terminal_signals(key);

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
      print_list(le, choices + offset, bh, ++index, sel_idx, offset);
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
      print_list(le, choices + offset, bh, --index, sel_idx, offset);
      break;
    case SY_ENTER_KEY:
      if (le->max_select > 1 && sy_array_len(sel_idx) < le->min_select) {
        break;
      }
      goto end;
    case ' ': {
      if (le->max_select < 2) {
        break;
      }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-conversion"
      size_t idx = sy_array_indexof(sel_idx, index + offset);
#pragma clang diagnostic pop
      if (idx != -1)
        sy_array_remove_index(sel_idx, idx);
      else {
        if (le->max_select <= sy_array_len(sel_idx))
          break;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-conversion"
        sy_array_append(sel_idx, index + offset);
#pragma clang diagnostic pop
      }
      print_list(le, choices + offset, bh, index, sel_idx, offset);
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
  sy_term_cursor_buf_pos_set(buffer, le->row, le->col);
  sy_buffer_write(buffer, STDOUT_FILENO);
  sy_term_cursor_show();

  return return_result(sel_idx);
}

void sy_term_list_res_free(sy_list_edit_res_t *res) {
  if (!res)
    return;
  free(res->indexes);
  free(res);
}