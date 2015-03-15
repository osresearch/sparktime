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

#define SPARK_CORE
#ifdef SPARK_CORE
// spark core
#define LED_LATCH	A1
#define LED_CLOCK2	A2
#define LED_CLOCK1	A3
#define LED_DATA	A5
#define LED_ENABLE	A6
#define ROW0		D0
#define ROW1		D1
#define ROW2		D2
#define ROW3		D3
#define ROW4		D4
#define ROW5		D5
#define ROW6		D6
#else
// teensy 3.1
#define LED_LATCH	10
#define LED_CLOCK2	 9
#define LED_CLOCK1	 8
#define LED_DATA	 6
#define LED_ENABLE	 5

#define ROW0		14
#define ROW1		15
#define ROW2		17
#define ROW3		18
#define ROW4		19
#define ROW5		20
#define ROW6		21
#endif

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

void setup()
{
	pinMode(ROW0, INPUT);
	pinMode(ROW1, INPUT);
	pinMode(ROW2, INPUT);
	pinMode(ROW3, INPUT);
	pinMode(ROW4, INPUT);
	pinMode(ROW5, INPUT);
	pinMode(ROW6, INPUT);

	digitalWrite(ROW0, 0);
	digitalWrite(ROW1, 0);
	digitalWrite(ROW2, 0);
	digitalWrite(ROW3, 0);
	digitalWrite(ROW4, 0);
	digitalWrite(ROW5, 0);
	digitalWrite(ROW6, 0);

	pinMode(LED_DATA, OUTPUT);
	pinMode(LED_LATCH, OUTPUT);
	pinMode(LED_ENABLE, OUTPUT);
	pinMode(LED_CLOCK1, OUTPUT);
	pinMode(LED_CLOCK2, OUTPUT);

	for (int y = 0 ; y < HEIGHT ; y++)
	{
		for (int x = 0 ; x < WIDTH ; x++)
		{
			fb[y][x] = (x % 8) < y ? 0 : 1;
			//fb[y][x] = x < WIDTH/2; //(x^y) & 1;
		}
	}
}


static inline void
row(
	const int row_pin,
	const uint8_t * const data,
	const uint8_t bright,
	const unsigned delay
)
{
	// clock 1 drops first, starting the cycle
	digitalWrite(LED_CLOCK1, 0);
	//delayMicroseconds(10);

	// a few microseconds later latch goes low
	digitalWrite(LED_LATCH, 0);


	// then the evens are clocked out
	for (unsigned i = 0 ; i < WIDTH ; i+=2)
	{
		fast_write(LED_CLOCK1, 1);
		fast_write(LED_DATA, data[i] > bright ?  1 : 0);
		fast_write(LED_CLOCK1, 0);
	}

	// after the evens clock2 goes HIGH
	digitalWrite(LED_CLOCK2, 1);
	delayMicroseconds(1);

	// and then the evens are clocked out
	for (unsigned i = 1 ; i < WIDTH ; i+=2)
	{
		fast_write(LED_CLOCK2, 1);
		fast_write(LED_DATA, data[i] > bright ?  1 : 0);
		fast_write(LED_CLOCK2, 0);
	}

	// always return the data pin to zero
	digitalWrite(LED_DATA, 0);

	//// enable goes high a few microseconds before the clocks and latch
	//digitalWrite(LED_ENABLE, 1);

	// latch goes high
	digitalWrite(LED_LATCH, 1);
	//delayMicroseconds(1);


	// then the reset the clocks to their idle state
	digitalWrite(LED_CLOCK1, 1);
	digitalWrite(LED_CLOCK2, 0);

	// turn on the pin
	digitalWrite(LED_ENABLE, 0);
	pinMode(row_pin, OUTPUT);

	//delay(100);
	delayMicroseconds(delay);

	// turn off the pin
	pinMode(row_pin, INPUT);

	// disable the output
	digitalWrite(LED_ENABLE, 1);
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
}

void binary_loop()
{
	row(ROW0, fb[0], 0, 400);
	row(ROW1, fb[1], 0, 400);
	row(ROW2, fb[2], 0, 400);
	row(ROW3, fb[3], 0, 400);
	row(ROW4, fb[4], 0, 400);
	row(ROW5, fb[5], 0, 400);
	row(ROW6, fb[6], 0, 400);
}

void loop()
{
#ifdef SPARK_CORE
		binary_loop();
#else
		pwm_loop();
#endif
}
