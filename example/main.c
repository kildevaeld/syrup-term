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

  sy_term_style_t style = {.normal = SY_WHITE,
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

  sy_list_edit_t cfg;

  char *choices[21];
  sy_buffer_t *buf = sy_buffer_alloc();
  for (int i = 0; i < 20; i++) {
    sy_buffer_clear(buf);
    sy_buffer_utf8_appendf(buf, "item %i", i);
    choices[i] = sy_buffer_string(buf);
  }
  choices[20] = "Abba";
  sy_term_list_edit_init(&cfg);
  cfg.col = 10;
  cfg.highlight = SY_MAGENTA;
  cfg.selected = 'v';
  cfg.unselected = 'x';
  cfg.max_select = 1;
  cfg.min_select = 1;
  cfg.clear = true;
  sy_term_list_edit_read(&cfg, choices, 21);

  return 0;
}