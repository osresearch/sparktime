/** \file
 * Interface a sparkcore with the SolderTime Desk Clock
 * Pin diagram is at:
 * https://github.com/spark/firmware/blob/master/src/spark_wiring.cpp#L46
 *
 * pin 7 to vin.
 * pin 8 and 22 to gnd
 */
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


#define ROW_MASK	(0 \
	| (1<<0) \
	| (1<<1) \
	| (1<<3) \
	| (1<<4) \
	| (1<<5) \
	| (1<<6) \
	| (1<<7) \
	)

#define COL_SHIFT	13
#define COL_MASK	(0 \
	| (1 << 13) \
	| (1 << 14) \
	| (1 << 15) \
	)

#define SEL_SHIFT	0
#define SEL_MASK	(0 \
	| (1 << 0) \
	| (1 << 1) \
	)

static uint8_t fb[WIDTH][HEIGHT];

// STM pins mapping has the GPIO structures
extern STM32_Pin_Info PIN_MAP[];
static GPIO_TypeDef * gpio_a;
static GPIO_TypeDef * gpio_b;

void
ledmatrix_setup()
{
	gpio_a = PIN_MAP[A0].gpio_peripheral;
	gpio_b = PIN_MAP[D0].gpio_peripheral;

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
	const STM32_Pin_Info * const p = &PIN_MAP[pin];

	if (value == LOW)
	{
		p->gpio_peripheral->BRR = p->gpio_pin;
	} else
	{
		p->gpio_peripheral->BSRR = p->gpio_pin;
	}
}


static void
gpio_set(
	GPIO_TypeDef * const g,
	const uint16_t bits,
	const uint16_t mask
)
{
	uint32_t one_bits = bits & mask;
	uint32_t zero_bits = (~bits) & mask;

	// BSRR has both set and reset functionality, with the bottom
	// 16-bits for set, and the top 16-bits for clear.
	g->BSRR = 0
		| (one_bits << 0)
		| (zero_bits << 16)
		;
}


static void
row_output(
	const uint16_t bits
)
{
	gpio_set(gpio_b, bits, ROW_MASK);
}
	

// disable the output
static void
col_disable(void)
{
	gpio_set(gpio_a, 3 << SEL_SHIFT, SEL_MASK);
}


// configure the column output, but select the no-decoder
static void col_output(uint8_t col)
{
	// clear all the outputs
	row_output(0);

	// select the correct output bank
	static const uint8_t sel[] = { 1, 2, 0 };
	const uint8_t bank = sel[col / 8];

	// select the correct column
	const uint8_t col_rank = col % 8;
    
	// write the column and bank selector to the gpio
	const uint16_t bits = 0
		| col_rank << COL_SHIFT
		| bank << SEL_SHIFT
		;

	gpio_set(gpio_a, bits, COL_MASK | SEL_MASK);
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
			uint16_t bits = 0
				| (b[0] > bright) << 0
				| (b[1] > bright) << 1
				| (b[2] > bright) << 3
				| (b[3] > bright) << 4
				| (b[4] > bright) << 5
				| (b[5] > bright) << 6
				| (b[6] > bright) << 7
				;
			row_output(bits);
			//delayMicroseconds(8);
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
