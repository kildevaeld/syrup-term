#include "private.h"
#include <signal.h>
#include <stdlib.h>
#include <syrup/term.h>

static propagate_signal(int sig) {
  pid_t pid = getpid();
  kill(pid, sig);
}

void handle_terminal_signals(int key) {
  switch (key) {
  case SY_CTRL_KEY('c'):
    sy_term_cursor_show();
    propagate_signal(SIGINT);
    break;
  case SY_CTRL_KEY('z'):
    sy_term_cursor_show();
    propagate_signal(SIGTSTP);
    break;
  }
}