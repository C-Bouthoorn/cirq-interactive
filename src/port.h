#ifndef PORT_H
#define PORT_H


#include <stdbool.h>

#include "chip.h"


typedef enum {
  PT_INPUT,
  PT_OUTPUT,
} port_type_t;

typedef struct port {
  chip_t *chip;
  struct wire *wire;

  port_type_t type;
  bool state;
} port_t;


void port_update(port_t *port, bool state, void *grid, bool print);

void port_init(port_t *port);
void port_print(port_t *port);
void port_free(port_t *port);


#endif
