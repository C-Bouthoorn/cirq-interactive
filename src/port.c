#include "port.h"

#include "cirq/assert.h"
#include "cirq/benchmark.h"

#include "wire.h"


void port_update(port_t *port, bool state, void *grid, bool print) {
  FUNC_START();

  assert_not_null(port);
  assert_not_null(port->chip);
  assert_not_null(port->wire);
  assert_not_null(grid);


  if (port->state != state) {
    port->state = state;

    chip_update(port->chip, grid, print);

    if (port->wire->state != state) {
      wire_update(port->wire, state, grid, print);
    }
  }

  FUNC_END();
}


void port_init(port_t *port) {
  FUNC_START();

  port->chip = NULL;
  port->wire = NULL;

  port->type = PT_INPUT;
  port->state = false;

  FUNC_END();
}


void port_print(port_t *port) {
  FUNC_START();

  assert_not_null(port);

  printf("%s%s" C_RESET, port->state ? C_LIGHT_GREEN : C_LIGHT_GRAY, port->type == PT_INPUT ? ">" : "<");

  FUNC_END();
}


void port_free(port_t *port) {
  FUNC_START();

  assert_not_null(port);

  FUNC_END();
}
