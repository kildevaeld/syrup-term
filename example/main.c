#include <syrup/term.h>

int main() {

  sy_term_clear_screen();

  sy_term_cursor_pos_set(10, 10);
  write(STDOUT_FILENO, "Hello", 5);
  sleep(1);
  sy_term_cursor_pos_set(10, 10);
  write(STDOUT_FILENO, "World", 5);

  return 0;
}