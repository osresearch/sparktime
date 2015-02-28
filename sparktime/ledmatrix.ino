/** \file
 * Interface a sparkcore with the SolderTime Desk Clock
 * Pin diagram is at:
 * https://github.com/spark/firmware/blob/master/src/spark_wiring.cpp#L46
spark	gpio	function
a6	b0	row 0
a7	b1	row 1
d4	b3	row 2
d3	b4	row 3
d2	b5	row 4
d1	b6	row 5
d0	b7	row 6

a0	a0	sel a
a1	a1	sel b
a2	a4
a3	a5
a4	a6
a5	a7
d7	a13	col a
d6	a14	col b
d5	a15	col c


 */
#include "sparktime.h"

#define	ROW0	A6	// gpio b0, pin 14
#define	ROW1	A7	// gpio b1, pin 15
#define	ROW2	D4	// gpio b3, pin 16
#define	ROW3	D3	// gpio b4, pin 17
#define	ROW4	D2	// gpio b5, pin 18
#define	ROW5	D1	// gpio b6, pin 19
#define	ROW6	D0	// gpio b7, pin 9

#define COL0	D7	// gpio a13, pin 6
#define COL1	D6	// gpio a14, pin 11
#define COL2	D5	// gpio a15, pin 12

#define SEL0	A0	// gpio a0, pin 25
#define SEL1	A1	// gpio a1, pin 26

uint8_t fb[WIDTH][HEIGHT];

void
ledmatrix_setup()
{
	pinMode(COL0, OUTPUT); // col b0
	pinMode(COL1, OUTPUT); // col b1
	pinMode(COL2, OUTPUT); // col b2
	pinMode(SEL0, OUTPUT); // S0
	pinMode(SEL1, OUTPUT); // S1
    
	pinMode(ROW0, OUTPUT); // row0 .. row6
	pinMode(ROW1, OUTPUT);
	pinMode(ROW2, OUTPUT);
	pinMode(ROW3, OUTPUT);
	pinMode(ROW4, OUTPUT);
	pinMode(ROW5, OUTPUT);
	pinMode(ROW6, OUTPUT);
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
	fast_write(SEL0, 1);
	fast_write(SEL1, 1);
}


// configure the column output, but select the no-decoder
static void col_output(uint8_t col)
{
	// clear all the outputs
	fast_write(ROW0, 0);
	fast_write(ROW1, 0);
	fast_write(ROW2, 0);
	fast_write(ROW3, 0);
	fast_write(ROW4, 0);
	fast_write(ROW5, 0);
	fast_write(ROW6, 0);

	// select the correct column
	const uint8_t col_rank = col % 8;
	fast_write(COL0, col_rank & 1);
	fast_write(COL1, col_rank & 2);
	fast_write(COL2, col_rank & 4);

	// select the correct output bank
	static const uint8_t sel[] = { 1, 2, 0 };

	const uint8_t bank = sel[col / 8];
    
	fast_write(SEL0, bank & 1);
	fast_write(SEL1, bank & 2);
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
			fast_write(ROW0, b[0] > bright);
			fast_write(ROW1, b[1] > bright);
			fast_write(ROW2, b[2] > bright);
			fast_write(ROW3, b[3] > bright);
			fast_write(ROW4, b[4] > bright);
			fast_write(ROW5, b[5] > bright);
			fast_write(ROW6, b[6] > bright);
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
