#ifndef TERMINAL_CONTROL_H
#define TERMINAL_CONTROL_H


#include <stdbool.h>


void enable_raw_input(bool disable_echo);
void reset_input(void);

void moveCursorUp(int amount);
void moveCursorDown(int amount);
void moveCursorRight(int amount);
void moveCursorLeft(int amount);
void moveCursorToBegin(void);
void clearLine(void);


#endif
