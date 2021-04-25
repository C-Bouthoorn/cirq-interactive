#include "grid.h"

#include <time.h>

#include "cirq/color.h"
#include "cirq/assert.h"
#include "cirq/benchmark.h"

#include "utils.h"


void grid_init(grid_t *grid, uint8_t width, uint8_t height) {
  FUNC_START();

  grid->width = width;
  grid->height = height;
  grid->fields = malloc(width * height * sizeof(grid_field_t*));


  srand((unsigned int) time(NULL));

  for (size_t i = 0; i < width * height; i++) {
    grid_field_t *gf = malloc(sizeof(grid_field_t));


    gf->type = GF_WIRE;

    wire_t *wire = malloc(sizeof(wire_t));
    wire_init(wire);
    wire->i = i;


    /*
    // Init randomly
    int RAND_FACT = i < 100 ? 3 : 2;

    wire->left  = rand()/RAND_FACT < rand();
    wire->right = rand()/RAND_FACT < rand();
    wire->up    = rand()/RAND_FACT < rand();
    wire->down  = rand()/RAND_FACT < rand();
    */



    gf->wire = wire;

    grid->fields[i] = gf;
  }

  FUNC_END();
}


void grid_print(grid_t *grid) {
  FUNC_START();

  assert_not_null(grid);

  // printf("grid [%u, %u]\n", grid->width, grid->height);
  printf("\n");

  printf("   ");
  for (size_t x = 0; x < grid->width; x++) {
    printf("%c", toBase52((uint8_t) x));
  }
  printf("\n");


  for (size_t y = 0; y < grid->height; y++) {
    printf("%s ", LEFT_PAD_SPACE(y, 2));

    for (size_t x = 0; x < grid->width; x++) {
      grid_field_t *field = grid->fields[y * grid->width + x];

      switch (field->type) {
        case GF_CHIP:
          chip_print(field->chip);
          break;

        case GF_WIRE:
          wire_print(field->wire);
          break;

        case GF_EMPTY:
        //default:
          printf(" ");
          break;
      }
    }

    printf("\n");
  }

  FUNC_END();
}


void grid_free(grid_t *grid) {
  FUNC_START();

  assert_not_null(grid);

  for (size_t i = 0; i < grid->width * grid->height; i++) {
    grid_field_t *field = grid->fields[i];

    switch (field->type) {
      case GF_CHIP:
        chip_free(field->chip, true);
        free(field->chip);
        break;

      case GF_WIRE:
        wire_free(field->wire);
        free(field->wire);
        break;

      case GF_EMPTY:
      //default:
        break;
    }

    free(field);
  }

  free(grid->fields);

  FUNC_END();
}
