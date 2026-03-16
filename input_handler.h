#pragma once

// Wrapper for platform-specific keyboard hit detection.
int kbhit_wrapper();

// Wrapper for platform-specific character getting.
int getch_wrapper();

// RAII class to manage terminal state for non-blocking input.
class InputManager {
public:
    InputManager();
    ~InputManager();
};