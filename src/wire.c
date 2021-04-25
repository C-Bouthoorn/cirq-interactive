#include "wire.h"

#include <stdio.h>
#include <stdint.h>

#include "cirq/color.h"
#include "cirq/assert.h"
#include "cirq/benchmark.h"

#include "grid.h"


#define WIRE(_i) ((wire_t*) grid->fields[_i]->wire)


void wire_update(wire_t *wire, bool state, void *_grid, bool print) {
  FUNC_START();

  grid_t *grid = (grid_t*) _grid;

  assert_not_null(wire);
  assert_not_null(grid);

  if (wire->state == state) {
    FUNC_END();
    return;
  }

  if (wire->port != NULL) { // && wire->port->type == PT_INPUT
    port_update(wire->port, state, grid, print);
  }


  wire->state = state;
  size_t i = wire->i;

  bool to_update[] = {
    // go right
    i % grid->width < grid->width - 1  && WIRE(i + 1)->state != state           && WIRE(i)->right && WIRE(i + 1)->left,
    // go left
    i % grid->width > 0                && WIRE(i - 1)->state != state           && WIRE(i)->left  && WIRE(i - 1)->right,
    // go up
    i / grid->width > 0                && WIRE(i - grid->width)->state != state && WIRE(i)->up    && WIRE(i - grid->width)->down,
    // go down
    i / grid->width < grid->height - 1 && WIRE(i + grid->width)->state != state && WIRE(i)->down  && WIRE(i + grid->width)->up,
  };

  if (! (to_update[0] || to_update[1] || to_update[2] || to_update[3])) {
    FUNC_END();
    return;
  }

  // printf("[%i] %x %x %x %x\n", i, to_update[0], to_update[1], to_update[2], to_update[3]);

  if (print) {
    grid_print(grid);
    // Move back
    printf("\033[%uA\r", grid->height + 2);

    // usleep(1000);
  }

  if (to_update[0]) wire_update(WIRE(i + 1), state, grid, print);
  if (to_update[1]) wire_update(WIRE(i - 1), state, grid, print);
  if (to_update[2]) wire_update(WIRE(i - grid->width), state, grid, print);
  if (to_update[3]) wire_update(WIRE(i + grid->width), state, grid, print);

  FUNC_END();
}


void wire_init(wire_t *wire) {
  FUNC_START();

  wire->left = false;
  wire->right = false;
  wire->up = false;
  wire->down = false;

  wire->state = false;

  wire->port = NULL;

  wire->i = 0;
  wire->symbol = NULL;

  FUNC_END();
}


void wire_print(wire_t *wire) {
  FUNC_START();

  assert_not_null(wire);

  if (wire->port != NULL) {
    port_print(wire->port);
  }
  else {
    int ix = wire->left << 3 | wire->right << 2 | wire->up << 1 | wire->down;

    char *color = wire->state ? C_LIGHT_GREEN : C_LIGHT_GRAY;

    printf("%s%s" C_RESET, color, wire->symbol != NULL ? wire->symbol : BOX_CHARS[ix]);
  }

  FUNC_END();
}


void wire_free(wire_t *wire) {
  FUNC_START();

  assert_not_null(wire);

  FUNC_END();
}
