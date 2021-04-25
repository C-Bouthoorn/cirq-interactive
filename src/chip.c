#include "chip.h"

#include "cirq/assert.h"
#include "cirq/benchmark.h"

#include "port.h"
#include "wire.h"



void chip_update(chip_t *chip, void *grid, bool print) {
  assert_not_null(chip);

  if (chip->type == CT_AND || chip->type == CT_OR || chip->type == CT_XOR) {
    assert_eq((int) chip->ports.amount, 3);

    // Get ports
    port_t *i0 = NULL;
    port_t *i1 = NULL;
    port_t *o0 = NULL;

    VEC_EACH(chip->ports, port_t *p) {
      if (p->type == PT_INPUT) {
        if (i0 == NULL) {
          i0 = p;
        }
        else if (i1 == NULL) {
          i1 = p;
        }
      }
      else if (p->type == PT_OUTPUT) {
        if (o0 == NULL) {
          o0 = p;
        }
      }
    }


    // Calculate new state
    bool newstate = false;
    if (chip->type == CT_AND) {
      newstate = i0->state & i1->state;
    }
    else if (chip->type == CT_OR) {
      newstate = i0->state | i1->state;
    }
    else if (chip->type == CT_XOR) {
      newstate = i0->state ^ i1->state;
    }

    // Apply new state
    wire_update(o0->wire, newstate, grid, print);
  }
}


void chip_init(chip_t *chip) {
  FUNC_START();

  vector_init(&chip->ports, BUF_SIZE);
  chip->name = "CHIP";
  chip->type = CT_OR;

  FUNC_END();
}


void chip_print(chip_t *chip) {
  FUNC_START();

  assert_not_null(chip);

  size_t amount = chip->ports.amount;

  printf("%s [%s] with %lu %s", chip->name, CT_NAMES[chip->type], amount, amount == 1 ? "port" : "ports");

  FUNC_END();
}


void chip_free(chip_t *chip, bool free_ports) {
  FUNC_START();

  assert_not_null(chip);

  if (free_ports) {
    VEC_EACH(chip->ports, port_t *port) {
      free(port);
    }
  }

  vector_free(&chip->ports);

  FUNC_END();
}
