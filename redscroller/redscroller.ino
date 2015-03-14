/*
 * 18 * 5x7 modules = 90x7 screen
 * A0 data
 * A1 enable?
 * A2 latch?
 * A3 clock1
 * A4 clock2
 *
 * original total time per row: 1 ms
 */

#define LED_DATA	A0
#define LED_ENABLE	A1
#define LED_LATCH	A2
#define LED_CLOCK1	A4
#define LED_CLOCK2	A3

#define WIDTH 90
#define HEIGHT 7

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
	const STM32_Pin_Info * const p = &PIN_MAP[pin];

	if (value == LOW)
	{
		p->gpio_peripheral->BRR = p->gpio_pin;
	} else
	{
		p->gpio_peripheral->BSRR = p->gpio_pin;
	}
}

void setup()
{
	pinMode(D0, INPUT);
	pinMode(D1, INPUT);
	pinMode(D3, INPUT);
	pinMode(D4, INPUT);
	pinMode(D5, INPUT);
	pinMode(D6, INPUT);
	pinMode(D7, INPUT);

	digitalWrite(D0, 0);
	digitalWrite(D1, 0);
	digitalWrite(D3, 0);
	digitalWrite(D4, 0);
	digitalWrite(D5, 0);
	digitalWrite(D6, 0);
	digitalWrite(D7, 0);

	pinMode(LED_DATA, OUTPUT);
	pinMode(LED_LATCH, OUTPUT);
	pinMode(LED_ENABLE, OUTPUT);
	pinMode(LED_CLOCK1, OUTPUT);
	pinMode(LED_CLOCK2, OUTPUT);

	for (int y = 0 ; y < HEIGHT ; y++)
	{
		for (int x = 0 ; x < WIDTH ; x++)
		{
			fb[y][x] = (x % 8) > y ? 0 : 1;
			//fb[y][x] = x < WIDTH/2; //(x^y) & 1;
		}
	}
}


static inline void
row(
	const int row_pin,
	const uint8_t * const data
)
{
	// clock 1 drops first, starting the cycle
	digitalWrite(LED_CLOCK1, 0);
	delayMicroseconds(10);

	// a few microseconds later latch goes low
	digitalWrite(LED_LATCH, 0);


	// then the evens are clocked out
	for (unsigned i = 0 ; i < WIDTH ; i+=2)
	{
		fast_write(LED_CLOCK1, 1);
		fast_write(LED_DATA, data[i] ?  1 : 0);
		fast_write(LED_CLOCK1, 0);
	}

	// after the evens clock2 goes HIGH
	digitalWrite(LED_CLOCK2, 1);
	delayMicroseconds(1);

	// and then the evens are clocked out
	for (unsigned i = 1 ; i < WIDTH ; i+=2)
	{
		fast_write(LED_CLOCK2, 1);
		fast_write(LED_DATA, data[i] ?  1 : 0);
		fast_write(LED_CLOCK2, 0);
	}

	// always return the data pin to zero
	digitalWrite(LED_DATA, 0);

	//// enable goes high a few microseconds before the clocks and latch
	//digitalWrite(LED_ENABLE, 1);

	// latch goes high
	digitalWrite(LED_LATCH, 1);
	delayMicroseconds(1);

	// then the reset the clocks to their idle state
	digitalWrite(LED_CLOCK1, 1);
	digitalWrite(LED_CLOCK2, 0);

	// turn on the pin
	digitalWrite(row_pin, 0);
	pinMode(row_pin, OUTPUT);

	//delay(100);
	digitalWrite(LED_ENABLE, 0);
	delayMicroseconds(400);

	// turn off the pin
	pinMode(row_pin, INPUT);

	// disable the output
	digitalWrite(LED_ENABLE, 1);
	//delay(100);
}


void loop()
{
	row(D0, fb[1]);
	row(D1, fb[2]);
	row(D3, fb[3]);
	row(D4, fb[4]);
	row(D5, fb[5]);
	row(D6, fb[6]);
	row(D7, fb[0]);
}
