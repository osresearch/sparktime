#ifndef _sparktime_h_
#define _sparktime_h_

#include <stdint.h>

#define ASCII_OFFSET 0x20
#define DIGIT_OFFSET 95

#define WIDTH 90
#define HEIGHT 7


extern uint8_t LETTERS[][5];

extern void
ledmatrix_setup();

extern void
ledmatrix_draw();


extern void
ledmatrix_set_col(
	const uint8_t col,
	const uint8_t bits,
	const uint8_t bright
);


extern void
ledmatrix_set(
	const uint8_t col,
	const uint8_t row,
	const uint8_t bright
);


extern void
draw_string(
	const char outText[]
);


void
draw_char(
	unsigned col,
	const char c
);

void
draw_small_digit(
	uint8_t column,
	unsigned digit,
	unsigned blinking
);

extern void
draw_time(
	uint8_t dig1,
	uint8_t dig2,
	uint8_t dig3,
	uint8_t dig4
);

extern void
draw_clear();

#endif
