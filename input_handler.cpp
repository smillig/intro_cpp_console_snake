#include "input_handler.h"

#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>

// On Windows, these are no-ops as conio handles it.
void init_input() {}
void restore_input() {}

int kbhit_wrapper() {
    return _kbhit();
}

int getch_wrapper() {
    return _getch();
}

#else // For Linux/macOS

#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstdio>

static struct termios old_tio;

// Initializes terminal for non-canonical, non-echoing input
void init_input() {
    tcgetattr(STDIN_FILENO, &old_tio);
    struct termios new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

// Restores original terminal settings
void restore_input() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

int kbhit_wrapper() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

int getch_wrapper() {
    return getchar();
}

#endif

InputManager::InputManager() { init_input(); }
InputManager::~InputManager() { restore_input(); }