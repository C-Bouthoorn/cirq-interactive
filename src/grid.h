#ifndef GRID_H
#define GRID_H


#include <stdint.h>

#include "cirq/vector.h"

#include "chip.h"
#include "wire.h"


enum grid_field_type {
  GF_EMPTY,
  GF_CHIP,
  GF_WIRE,
};


typedef struct {
  enum grid_field_type type;

  union {
    chip_t *chip;
    wire_t *wire;
  };
} grid_field_t;


typedef struct grid {
  uint8_t width;
  uint8_t height;

  grid_field_t **fields;
} grid_t;


void grid_init(grid_t *grid, uint8_t width, uint8_t height);
void grid_print(grid_t *grid);
void grid_free(grid_t *grid);


#endif
