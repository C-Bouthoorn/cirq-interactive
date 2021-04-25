#ifndef UTILS_H
#define UTILS_H


#include <stdbool.h>
#include <stdint.h>


char *__LEFT_PAD_SPACE(char *dest, double x, unsigned int width);
#define LEFT_PAD_SPACE(x, y) __LEFT_PAD_SPACE((char [32]){""}, (x), (y))

char toBase52(uint8_t x);
uint8_t fromBase52(char x);

#define msleep(s) usleep(s * 1000)


#endif
