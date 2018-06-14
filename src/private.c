#include "private.h"
#include <stdlib.h>
#include <syrup/term.h>

void handle_terminal_signals(int key) {
  switch (key) {
  case SY_CTRL_KEY('c'):
    sy_term_cursor_show();
    exit(2);
  }
}