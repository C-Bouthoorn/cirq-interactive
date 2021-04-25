#ifndef CHIP_H
#define CHIP_H

// #include "vector.h"
#include "cirq/vector.h"

enum chiptype {
  CT_AND, CT_OR, CT_XOR,
};

const static char *CT_NAMES[] = {
  "AND", "OR", "XOR"
};


typedef struct chip {
  char *name;
  vector_t ports; // vector<port_t>

  enum chiptype type;
} chip_t;


void chip_update(chip_t *chip, void *grid, bool print);

void chip_init(chip_t *chip);
void chip_print(chip_t *chip);
void chip_free(chip_t *chip, bool free_ports);


#endif
