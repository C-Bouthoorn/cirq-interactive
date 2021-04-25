#include "terminal_control.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>


// See: https://stackoverflow.com/a/1798833
static struct termios oldt, newt;

void enable_raw_input(bool disable_echo) {
  /*tcgetattr gets the parameters of the current terminal
  STDIN_FILENO will tell tcgetattr that it should write the settings
  of stdin to oldt*/
  tcgetattr(STDIN_FILENO, &oldt);
  /*now the settings will be copied*/
  newt = oldt;

  /*ICANON normally takes care that one line at a time will be processed
  that means it will return if it sees a "\n" or an EOF or an EOL*/
  newt.c_lflag &= ~(ICANON | (disable_echo & ECHO));

  /*Those new settings will be set to STDIN
  TCSANOW tells tcsetattr to change attributes immediately. */
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void reset_input(void) {
  /*restore the old settings*/
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}


void moveCursorUp(int amount) {
  printf("\033[%uA", amount);
}

void moveCursorDown(int amount) {
  printf("\033[%uB", amount);
}

void moveCursorRight(int amount) {
  printf("\033[%uC", amount);
}

void moveCursorLeft(int amount) {
  printf("\033[%uD", amount);
}

void moveCursorToBegin(void) {
  printf("\r");
}

void clearLine(void) {
  printf("\033[2K");
}
