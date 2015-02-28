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


/** Faster output to an IO pin.
 * We've already verified that things are configured;
 * just write directly to the mapped pin's gpio line.
 */
static void
fast_write(
	const uint16_t pin,
	const uint8_t value
)
{
	extern STM32_Pin_Info PIN_MAP[];
	const STM32_Pin_Info * const p = &PIN_MAP[pin];

	if (value == LOW)
	{
		p->gpio_peripheral->BRR = p->gpio_pin;
	} else
	{
		p->gpio_peripheral->BSRR = p->gpio_pin;
	}
}


// disable the output
static void
col_disable(void)
{
	fast_write(A3, 1);
	fast_write(A4, 1);
}


// configure the column output, but select the no-decoder
static void col_output(uint8_t col)
{
	// clear all the outputs
	fast_write(D0, 0);
	fast_write(D1, 0);
	fast_write(D2, 0);
	fast_write(D3, 0);
	fast_write(D4, 0);
	fast_write(D5, 0);
	fast_write(D6, 0);

	// select the correct column
	const uint8_t col_rank = col % 8;
	fast_write(A0, col_rank & 1);
	fast_write(A1, col_rank & 2);
	fast_write(A2, col_rank & 4);

	// select the correct output bank
	static const uint8_t sel[] = { 1, 2, 0 };

	const uint8_t bank = sel[col / 8];
    
	fast_write(A3, bank & 1);
	fast_write(A4, bank & 2);
}


/** Draw the entire matrix;
 * takes about 20*200 us == 4ms.
 */
void
ledmatrix_draw()
{
	for(uint8_t col=0 ; col < WIDTH ; col++)
	{
		col_output(col);
		const uint8_t * const b = fb[col];
		for (uint8_t bright = 1 ; bright < 240 ; bright += 8)
		{
			fast_write(D0, b[0] > bright);
			fast_write(D1, b[1] > bright);
			fast_write(D2, b[2] > bright);
			fast_write(D3, b[3] > bright);
			fast_write(D4, b[4] > bright);
			fast_write(D5, b[5] > bright);
			fast_write(D6, b[6] > bright);
		}

		col_disable();
	}

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
