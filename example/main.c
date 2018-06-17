#include <stdlib.h>
#include <syrup/array-list.h>
#include <syrup/buffer.h>
#include <syrup/form.h>
#include <syrup/line-edit.h>
#include <syrup/list-edit.h>
#include <syrup/term.h>
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h> // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

#include <syrup/colors.h>

void sleep_ms(int milliseconds) // cross-platform sleep function
{
#ifdef WIN32
  Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
  struct timespec ts;
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds % 1000) * 1000000;
  nanosleep(&ts, NULL);
#else
  usleep(milliseconds * 1000);
#endif
}

int cmpstr(const void *a, const void *b) {
  const char *aa = (const char *)a;
  const char *bb = (const char *)b;
  return strcmp(aa, bb);
}

int main() {

  /*sy_term_cursor_hide();
  sy_buffer_t *buf = sy_buffer_alloc();

  int rows, cols;
  sy_term_size(&rows, &cols);

  int start = cols / 3;
  int rstart = 0;
  while (start >= 0) {
    sy_term_clear_screen_buf(buf);
    sy_term_cursor_buf_pos_set(buf, rstart < (rows / 2) ? rstart++ : rows / 2,
                               start--);
    sy_buffer_append_str(buf, "Hello, World!");
    sy_buffer_write(buf, STDOUT_FILENO);
    sy_buffer_clear(buf);
    sleep_ms(1000 / 60);
  }

  sy_term_cursor_buf_down(buf, 1);
  sy_term_cursor_buf_tostart(buf);
  sy_term_color(buf, SY_BACKGROUND | SY_CYAN);
  sy_buffer_append_str(buf, "Hello, you beautiful bastard!");
  sy_term_color_reset(buf);
  sy_term_cursor_buf_show(buf);

  sy_buffer_write(buf, STDOUT_FILENO);
  sy_term_cursor_buf_tostart(buf);

  sy_buffer_free(buf);*/

  /*sy_term_style_t style = {.normal = SY_RED, .input = SY_CYAN};

  char *choices[] = {"test", "mig"};
  sy_term_form_password(&style, "Pasword:", "*");

  sy_term_form_prompt(&style, "Name:");
  sy_term_form_confirm(&style, "Yes", false);
  sy_term_form_list(&style, "Hello:", choices, 2);*/

  sy_term_style_t style = {.normal = SY_BOLD | SY_HIGHDENS,
                           .input = SY_YELLOW,
                           .value = SY_CYAN | SY_HIGHDENS,
                           .muted = SY_GREEN};

  /*sy_term_form_input_cfg input_cfg = {.msg = "Name",
                                      .style = &style,
                                      //.defaults = "rasmus",
                                      .required = true,
                                      .echo = true,
                                      .clear = false};

  char *pass = sy_term_form_input(&input_cfg);
  printf("pass %s\n", pass);

  sy_term_form_confirm_cfg confirm_cfg = {.msg = "Name",
                                          .style = &style,
                                          //.defaults = "rasmus",
                                          .required = true,
                                          .clear = false};

  bool confirm = sy_term_form_confirm(&confirm_cfg);*/

  /*sy_list_edit_t cfg;

  sy_array_t *choices = sy_array_new((sy_array_comparator_fn)strcmp);
  sy_buffer_t *buf = sy_buffer_alloc();
  for (int i = 0; i < 40; i++) {
    sy_buffer_clear(buf);
    sy_buffer_utf8_appendf(buf, "item %i", i);
    // choices[i] = sy_buffer_string(buf);
    sy_array_append(choices, sy_buffer_string(buf));
  }

  sy_buffer_free(buf);

  sy_array_append(choices, strdup("Abbacadabra"));
  sy_array_sort(choices);
  // choices[20] = "Abba";
  sy_term_list_edit_init(&cfg);
  cfg.col = 10;
  cfg.highlight = SY_MAGENTA;
  cfg.selected = "◉ ";
  cfg.unselected = "◯ ";
  cfg.max_select = 2;
  cfg.min_select = 1;

  sy_list_edit_res_t *res = sy_term_list_edit_read(
      &cfg, (char **)sy_array_raw(choices), sy_array_len(choices));

  if (res) {
    printf("selected: ");
    for (int i = 0; i < res->len; i++) {
      printf("%s", sy_array_get(choices, res->indexes[i]));
      if (i == res->len - 1)
        printf("\n");
      else
        printf(", ");
    }
  }

  sy_term_list_res_free(res);

  sy_array_foreach(choices, (sy_array_iterate_fn)free);
  sy_array_free(choices);

  return 0;*/

  sy_array_t *choices = sy_array_new((sy_array_comparator_fn)strcmp);
  sy_buffer_t *buf = sy_buffer_alloc();
  for (int i = 0; i < 100; i++) {
    sy_buffer_clear(buf);
    sy_buffer_utf8_appendf(buf, "item %i", i);
    // choices[i] = sy_buffer_string(buf);
    sy_array_append(choices, sy_buffer_string(buf));
  }

  sy_buffer_free(buf);

  sy_array_append(choices, strdup("Abbacadabra"));
  sy_array_sort(choices);

  sy_term_enable_raw_mode();
  int row, col;
  sy_term_cursor_pos_get(&row, &col);
  sy_term_disable_raw_mode();
  sy_term_form_select_cfg cfg = {.msg = "Hello",
                                 .selected = "◉ ",
                                 .unselected = "◯ ",
                                 .row = row,
                                 .col = col,
                                 .max_selected = 1,
                                 .max_height = 10,
                                 .style = &style};

  sy_term_form_select(&cfg, sy_array_raw(choices), sy_array_len(choices));
  sy_array_foreach(choices, free);
  sy_array_free(choices);
}