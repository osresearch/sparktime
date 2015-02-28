#include "sparktime.h"

void
draw_char(
	unsigned col,
	const char c
)
{
	for (uint8_t y=0 ; y<5 ; y++)
		ledmatrix_set_col(col++, LETTERS[c - ASCII_OFFSET][y], 0xFF);
}


/*
 * Output 4 Characters to Display
 */
void
draw_string(
	const char outText[]
)
{
	for (int i=0 ; i < 4 ; i++)
		draw_char(5*i+1, outText[i]);
}


static void
draw_small_digit(
	uint8_t column,
	unsigned digit,
	unsigned blinking
)
{
	for (unsigned i=0 ; i < 4 ; i++)
	{
		ledmatrix_set_col(
			column+i,
			LETTERS[digit+DIGIT_OFFSET][i+1],
			0xFF //blinkON && blinking ? 0 : 0xFF
		);
	}
}


/*
 * Clear LED Matrix
 */
void
draw_clear()
{
	for (int i=0 ; i<WIDTH ; i++)
		ledmatrix_set_col(i, 0, 0);
}



/**
 * Display the four digit time with small characters.
 *
 *
 * Fills diplay with formated time
 * Depending on postion of "1"s spacing is adjusted beween it and next digit
 * Blinks if it settng mode
 * displays AM/PM dot and Alarm on dot
 */
void
draw_time(
	uint8_t dig1,
	uint8_t dig2,
	uint8_t dig3,
	uint8_t dig4
)
{
	const int blinkHour = 0;
	const int blinkMin = 0;

	draw_small_digit( 2, dig1, blinkHour);
	draw_small_digit( 6, dig2, blinkHour);

	// the slowly flashing " : "
	static uint16_t bright = 0;
	uint8_t b = bright++ / 8;
	if (b >= 128)
		b = 0xFF - b;
	ledmatrix_set(10, 2, 2*b);
	ledmatrix_set(10, 4, 2*b);

	draw_small_digit(12, dig3, blinkMin);
	draw_small_digit(16, dig4, blinkMin);

#if 0
	AMPMALARMDOTS = 0;

	// Alarm dot (top left) Do not display while setting alarm
	if (ALARMON && (STATE == 1))
		bitSet(AMPMALARMDOTS,6);

	// AM / PM dot (bottom left) (Display or Set Time)
	if(PM_NotAM_flag && (STATE == 1 || STATE == 2) && TH_Not24_flag)
		bitSet(AMPMALARMDOTS,0);

	// AM / PM dot (bottom left) (Set Alarm Time)
	if(A_PM_NotAM_flag && (STATE == 3) && TH_Not24_flag)
		bitSet(AMPMALARMDOTS,0);
#endif
}
