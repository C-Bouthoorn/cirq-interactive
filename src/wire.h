#ifndef WIRE_H
#define WIRE_H


#include <stdbool.h>

#include "port.h"


const static char *BOX_CHARS[] = {
  // LRUD

  "·", // b0000
  "╷", // b0001
  "╵", // b0010
  "│", // b0011
  "╶", // b0100
  "┌", // b0101
  "└", // b0110
  "├", // b0111
  "╴", // b1000
  "┐", // b1001
  "┘", // b1010
  "┤", // b1011
  "─", // b1100
  "┬", // b1101
  "┴", // b1110
  "┼", // b1111
};


typedef struct wire {
  bool left;
  bool right;
  bool up;
  bool down;

  bool state;

  struct port *port;
  size_t i;
  char *symbol;
} wire_t;


//                                         v should be grid_t* but dependency cycles avoid that
void wire_update(wire_t *wire, bool state, void *grid, bool print);

void wire_init(wire_t *wire);
void wire_print(wire_t *wire);
void wire_free(wire_t *wire);


#endif
