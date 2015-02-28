#ifndef _sparktime_h_
#define _sparktime_h_

#include <stdint.h>

#define ASCII_OFFSET 0x20
#define DIGIT_OFFSET 95

#define WIDTH 20
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

#endif
