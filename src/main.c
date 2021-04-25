#include <unistd.h>

// NOTE: Needs `cirq` as a library
#include "cirq/benchmark.h"

#include "grid.h"
#include "utils.h"
#include "terminal_control.h"


#define WIRE(_i) ((wire_t*) grid->fields[_i]->wire)


void get_coor_from_stdin(uint8_t *x, uint8_t *y);
void get_coor_from_stdin(uint8_t *x, uint8_t *y) {
  // Read normally
  reset_input();

  // Read line
  char input[16];
  fgets(input, 16, stdin);
  for (size_t i = 0; i < 16; i++) {
    if (input[i] == '\r' || input[i] == '\n') {
      input[i] = 0;
      break;
    }
  }

  // Get coordinates
  *x = fromBase52(input[0]);
  *y = (uint8_t) atoi(input + 1);

  // Check output from atoi
  if (*y == 0 && strcmp(input + 1, "0") != 0) {
    *y = (uint8_t) -1;
  }

  // Reset terminal to raw input
  enable_raw_input(false);
}


void addTwoInputsAndOneOutputToChip(chip_t *chip);
void addTwoInputsAndOneOutputToChip(chip_t *chip) {
  for (size_t i = 0; i < 3; i++) {
    port_t *p = malloc(sizeof(port_t));
    port_init(p);

    p->type = PT_INPUT;
    if (i == 2) {
      p->type = PT_OUTPUT;
    }

    p->chip = chip;
    vector_push(&chip->ports, p);
  }
}


void initGrid(grid_t *grid, chip_t *or_chip, chip_t *and_chip);
void initGrid(grid_t *grid, chip_t *or_chip, chip_t *and_chip) {
  // TODO: Dynamic

  // Init grid
  for (int i = 0; i < grid->width * grid->height; i++) {
    int x = i % grid->width;
    int y = i / grid->width;

    if (
      (x < 10 && (y == 0 || y == 2 || y == 4 || y == 6))
      || (x > 12 && x < 22 && (y == 1 || y == 5))
      || (y == 3 && x <= 'v' - 'a')
    ) {
      WIRE(i)->left = WIRE(i)->right = 1;

      if (x == 0) {
        WIRE(i)->left = 0;
        if (y == 3) {
          WIRE(i)->down = 1;
        }
        else if (y == 4) {
          WIRE(i)->up = 1;
        }
      }
      else if (x == 'v' - 'a') {
        WIRE(i)->right = 0;
        if (y == 1) {
          WIRE(i)->down = 1;
        }
        else if (y == 3) {
          WIRE(i)->up = 1;
        }
      }

      if (x == 9 && y == 0) {
        WIRE(i)->port = or_chip->ports.items[0];
      }
      else if (x == 9 && y == 2) {
        WIRE(i)->port = or_chip->ports.items[1];
      }
      else if (x == 13 && y == 1) {
        WIRE(i)->port = or_chip->ports.items[2];
      }

      else if (x == 9 && y == 4) {
        WIRE(i)->port = and_chip->ports.items[0];
      }
      else if (x == 9 && y == 6) {
        WIRE(i)->port = and_chip->ports.items[1];
      }
      else if (x == 13 && y == 5) {
        WIRE(i)->port = and_chip->ports.items[2];
      }

      else {
        continue;
      }

      WIRE(i)->port->wire = WIRE(i);
    }

    WIRE(2 * grid->width + 'v' - 'a')->up = WIRE(2 * grid->width + 'v' - 'a')->down = 1;


    if (x == 'l' - 'a') {
      if (y == 1) {
        WIRE(i)->symbol = "|";
      }
      else if (y == 5) {
        WIRE(i)->symbol = "&";
      }
    }
  }
}


void enableDisableToggle(char c, grid_t *grid);
void enableDisableToggle(char c, grid_t *grid) {
  bool enable = c == 'e';
  bool disable = c == 'd';
  bool toggle = c == 't';

  clearLine();

  printf("\b%s: ", enable ? "enable" : disable ? "disable" : "toggle");

  uint8_t x, y;
  get_coor_from_stdin(&x, &y);

  if (y >= grid->height || x >= grid->width) {
    printf("invalid coordinate!\n");
    msleep(1000);

    moveCursorUp(2);
    moveCursorToBegin();
  }
  else {
    int ix = y * grid->width + x;
    printf("(%u %u)\n", x, y);

    // Go back to beginning of the grid
    // NOTE: Assuming a grid is printed
    moveCursorUp(grid->height + 2 + 2);
    moveCursorToBegin();

    if (enable || disable) {
      wire_update(WIRE(ix), enable, grid, true);
    }
    else if (toggle) {
      wire_update(WIRE(ix), ! WIRE(ix)->state, grid, true);
    }

    grid_print(grid);
  }

  msleep(1000);
  // Clear current line, go to next line, clear current line, go back one line
  // printf("\r\033[K\033[1B\033[K\033[1A");
  moveCursorToBegin(); clearLine(); moveCursorDown(1); clearLine(); moveCursorUp(1);
}


void randomise(grid_t *grid);
void randomise(grid_t *grid) {
  // Randomise grid
  printf("\rrandomise");

  for (size_t i = 0; i < grid->height * grid->width; i++) {
    wire_t *wire = grid->fields[i]->wire;

    wire->left  = rand()/2 < rand();
    wire->right = rand()/2 < rand();
    wire->up    = rand()/2 < rand();
    wire->down  = rand()/2 < rand();
  }

  // Go back to the beginning of the grid
  // NOTE: Assuming a grid is printed
  // printf("\033[%uA\r", grid->height + 2);
  moveCursorUp(grid->height + 2);
  moveCursorToBegin();

  grid_print(grid);

  msleep(1000);

  // Clear current line
  moveCursorToBegin();
  clearLine();
}


void reset(grid_t *grid);
void reset(grid_t *grid) {
  printf("\rreset");

  for (size_t i = 0; i < grid->height * grid->width; i++) {
    wire_t *wire = grid->fields[i]->wire;

    wire_update(wire, false, grid, false);
  }

  // Go back to the beginning of the grid
  // NOTE: Assuming a grid is printed
  moveCursorUp(grid->height + 2);
  moveCursorToBegin();

  grid_print(grid);

  msleep(1000);
  clearLine();
  moveCursorToBegin();
}


void handleInteractive(grid_t *grid, char c);
void handleInteractive(grid_t *grid, char c) {
  switch (c) {
    case 'e':
    case 'd':
    case 't':
      enableDisableToggle(c, grid);
      break;

    case 'r':
      randomise(grid);
      break;

    case 'R':
      reset(grid);
      break;

    case '\n':
      moveCursorUp(1);
      break;

    default:
      printf(": unknown command\r");
  }
}


void setupGrid(grid_t *grid, chip_t *or_chip, chip_t *and_chip);
void setupGrid(grid_t *grid, chip_t *or_chip, chip_t *and_chip) {
  grid_init(grid, 32, 8);

  chip_init(or_chip);
  chip_init(and_chip);

  or_chip->type = CT_OR;
  and_chip->type = CT_AND;

  addTwoInputsAndOneOutputToChip(or_chip);
  addTwoInputsAndOneOutputToChip(and_chip);

  initGrid(grid, or_chip, and_chip);
}


void startInteractive(grid_t *grid);
void startInteractive(grid_t *grid) {
  // Enable interactive mode
  enable_raw_input(false);

  while (true) {
    char c = (char) getchar();

    // Check for ^C or q to exit
    if (c == 3 || c == 'q') {
      printf("\rkthxbye\n");
      break;
    }


    handleInteractive(grid, c);
  }

  // Disable interactive mode
  reset_input();
}



int main() {
  BENCH_ON();
  FUNC_START();

  // Setup grid and chips
  grid_t grid;
  chip_t or_chip, and_chip;
  setupGrid(&grid, &or_chip, &and_chip);

  grid_print(&grid);

  // Start interactive mode
  startInteractive(&grid);

  // Free everything
  chip_free(&or_chip, true);
  chip_free(&and_chip, true);
  grid_free(&grid);
  FUNC_END();
}
