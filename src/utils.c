#include "utils.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "cirq/assert.h"
#include "cirq/benchmark.h"


#define ABS(x) ( ((x) < 0) ? -(x) : (x) )

char *__LEFT_PAD_SPACE(char *dest, double x, unsigned int width) {
	// Calculate width of number
	unsigned int num_width = 0;

	for (double y = ABS(x); y >= 1; y /= 10) {
		num_width++;
	}

	if (x < 0) {
		num_width++;
	}
	else if (x < 0.0001) { // x == 0
		num_width++;
	}

	// Add spaces
	if (width > num_width) {
		sprintf(dest, "%*s%.f", width - num_width, " ", x);
	}
	else {
		sprintf(dest, "%.f", x);
	}


	return dest;
}


char toBase52(uint8_t x) {
  return (char) (
    x < 26
      ? 'a' + x
      : (x < 52)
        ? x - 26 + 'A'
        : -1
    );
}


uint8_t fromBase52(char x) {
  return (uint8_t) (
    (x >= 'a' && x <= 'z')
      ? x - 'a'
      : (x >= 'A' && x <= 'Z')
        ? x - 'A' + 26
        : -1
  );
}
