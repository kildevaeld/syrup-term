#include <syrup/buffer.h>
#include <syrup/term.h>
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h> // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

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

  sy_term_cursor_hide();
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
  sy_buffer_append_str(buf, "Hello, you beautiful bastard!");
  sy_term_cursor_buf_show(buf);
  sy_buffer_write(buf, STDOUT_FILENO);

  sy_buffer_free(buf);

  return 0;
}