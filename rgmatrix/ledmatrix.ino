/*
 * 18 * 5x7 modules = 90x7 screen
 * On the spark core:
 * A0 data
 * A1 enable?
 * A2 latch?
 * A3 clock1
 * A4 clock2
 *
 * original total time per row: 1 ms
 */
#include "redscroller.h"

#define SPARK_CORE
#ifdef SPARK_CORE
// spark core
#define LED_CLOCK	A0
#define LED_LATCH	A1
#define LED_DATA1	A2
#define LED_DATA2	A3
#define LED_ENABLE	A4
#define ROW0		A5
#define ROW1		A6
#define ROW2		A7
#define ROW3		D0
#define UNKNOWN		D1
#else
// teensy 3.1
#error "teensy not support yet"
#endif

static uint8_t fb[HEIGHT][WIDTH];

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
#ifdef SPARK_CORE
	const STM32_Pin_Info * const p = &PIN_MAP[pin];

	if (value == LOW)
	{
		p->gpio_peripheral->BRR = p->gpio_pin;
	} else
	{
		p->gpio_peripheral->BSRR = p->gpio_pin;
	}
#else
	digitalWrite(pin, value);
#endif
}

void ledmatrix_setup()
{
	pinMode(ROW0, OUTPUT);
	pinMode(ROW1, OUTPUT);
	pinMode(ROW2, OUTPUT);
	pinMode(ROW3, OUTPUT);

	digitalWrite(ROW0, 0);
	digitalWrite(ROW1, 0);
	digitalWrite(ROW2, 0);
	digitalWrite(ROW3, 0);

	pinMode(LED_DATA1, OUTPUT);
	pinMode(LED_DATA2, OUTPUT);
	pinMode(LED_LATCH, OUTPUT);
	pinMode(LED_ENABLE, OUTPUT);
	pinMode(LED_CLOCK, OUTPUT);
	pinMode(UNKNOWN, OUTPUT);

	digitalWrite(LED_DATA1, 1);
	digitalWrite(LED_DATA2, 1);
	digitalWrite(LED_LATCH, 0);
	digitalWrite(LED_ENABLE, 0);
	digitalWrite(LED_CLOCK, 0);
	digitalWrite(UNKNOWN, 0);

	for (int y = 0 ; y < HEIGHT ; y++)
	{
		for (int x = 0 ; x < WIDTH ; x++)
		{
			fb[y][x] = 1; // (x % 8) < y ? 0 : 1;
			//fb[y][x] = x < WIDTH/2; //(x^y) & 1;
		}
	}
}


static inline void
draw_row(
	const unsigned row,
	const uint8_t * const data,
	const uint8_t bright,
	const unsigned delay
)
{
	// clock out both red and green data simultaneously
	for (unsigned i = 0 ; i < WIDTH ; i++)
	{
		fast_write(LED_CLOCK, 1);
		// inverted logic on LEDs
		fast_write(LED_DATA1, !data[i] ? 1 : 0); // green
		//fast_write(LED_DATA2, !data[i] ? 1 : 0); // red
		fast_write(LED_CLOCK, 0);
	}

	// always return the data pin to hi
	digitalWrite(LED_DATA1, 1);
	digitalWrite(LED_DATA2, 1);

	// turn off the current row
	digitalWrite(LED_ENABLE, 0);

	// latch the new data
	digitalWrite(LED_LATCH, 0);
	delayMicroseconds(1);
	digitalWrite(LED_LATCH, 1);

	// select the output row
	fast_write(ROW0, row & 1);
	fast_write(ROW1, row & 2);
	fast_write(ROW2, row & 4);
	fast_write(ROW3, row & 8);

	// and turn it on
	digitalWrite(LED_ENABLE, 1);

	//delayMicroseconds(delay/4);

	//delay(100);
}


static const unsigned bright[] = {
	1,
	30,
	60,
	90,
	120,
	160,
	250,
	400
};

void pwm_loop()
{
#if 0
	//for(unsigned x = 0; x < 256 ; x++)
	{
		for(unsigned i = 0; i < 8 ; i++)
		{
			row(ROW0, fb[0], i*256/8, bright[i]);
			row(ROW1, fb[1], i*256/8, bright[i]);
			row(ROW2, fb[2], i*256/8, bright[i]);
			row(ROW3, fb[3], i*256/8, bright[i]);
			row(ROW4, fb[4], i*256/8, bright[i]);
			row(ROW5, fb[5], i*256/8, bright[i]);
			row(ROW6, fb[6], i*256/8, bright[i]);
		}
	}
#endif
}

void binary_loop()
{
for(int j=0 ; j < 16 ; j++)
	for(unsigned i = 0 ; i < HEIGHT ; i++)
		draw_row(i, fb[i], 0, 400);
}

void ledmatrix_draw()
{
#ifdef SPARK_CORE
		binary_loop();
#else
		pwm_loop();
#endif
}


/** Set an entire column at once */
void
ledmatrix_set_col(
	const uint8_t col,
	const uint8_t row,
	const uint8_t bits,
	const uint8_t bright
)
{
/*
	uint8_t * p = &fb[0][WIDTH-1-col];
	for (uint8_t i = 0 ; i < HEIGHT, p += WIDTH ; i++)
		*p = (bits & (1 << i)) ? bright : 0;
*/
	for (uint8_t i = 0 ; i < 8 ; i++)
		ledmatrix_set(col, i+row, (bits & (1 << i)) ? bright : 0);
}


void
ledmatrix_set(
	const uint8_t col,
	const uint8_t row,
	const uint8_t bright
)
{
	fb[row][col] = bright;
}
