/** \file
 * Interface a sparkcore with the SolderTime Desk Clock
 */
#include "sparktime.h"

uint8_t fb[WIDTH][HEIGHT];

void
ledmatrix_setup()
{
	pinMode(A0, OUTPUT); // col b0
	pinMode(A1, OUTPUT); // col b1
	pinMode(A2, OUTPUT); // col b2
	pinMode(A3, OUTPUT); // S0
	pinMode(A4, OUTPUT); // S1
    
	pinMode(D0, OUTPUT); // row0 .. row6
	pinMode(D1, OUTPUT);
	pinMode(D2, OUTPUT);
	pinMode(D3, OUTPUT);
	pinMode(D4, OUTPUT);
	pinMode(D5, OUTPUT);
	pinMode(D6, OUTPUT);
    
	pinMode(D7, OUTPUT); // led
}


// configure the column output, but select the no-decoder
static void col_output(uint8_t col)
{
	col %= 8;
	digitalWrite(A3, 1);
	digitalWrite(A4, 1);
    
	digitalWrite(A0, col & 1);
	digitalWrite(A1, col & 2);
	digitalWrite(A2, col & 4);
}


static void col_enable(uint8_t col)
{
	static const uint8_t sel[] = { 1, 2, 0 };

	col = sel[col / 8];
    
	digitalWrite(A3, col & 1);
	digitalWrite(A4, col & 2);
}


/** Draw the entire matrix;
 * takes about 20*200 us == 4ms.
 */
void
ledmatrix_draw()
{
	for(uint8_t col=0 ; col <= WIDTH ; col++)
	{
		col_output(col);
		const uint8_t * const b = fb[col];
		digitalWrite(D0, b[0]);
		digitalWrite(D1, b[1]);
		digitalWrite(D2, b[2]);
		digitalWrite(D3, b[3]);
		digitalWrite(D4, b[4]);
		digitalWrite(D5, b[5]);
		digitalWrite(D6, b[6]);
		col_enable(col);
		delayMicroseconds(200);
	}

	col_enable(WIDTH+1);
}


/** Set an entire column at once */
void
ledmatrix_set_col(
	const uint8_t col,
	const uint8_t bits,
	const uint8_t bright
)
{
	uint8_t * const p = fb[col];
	for (uint8_t i = 0 ; i < HEIGHT ; i++)
		p[i] = (bits & (1 << i)) ? bright : 0;
}


void
ledmatrix_set(
	const uint8_t col,
	const uint8_t row,
	const uint8_t bright
)
{
	fb[col][row] = bright;
}
