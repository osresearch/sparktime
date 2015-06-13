#include "redscroller.h"
#define ONE_DAY_MILLIS (24*60*60*1000)

static int
show_string(String s)
{
	draw_clear();

	for(unsigned i = 0 ; i < s.length() && i*5 < 90 ; i++)
		draw_char(i*5, 0, s.charAt(i));

	return s.length();
}


static unsigned
draw_num(
	unsigned x,
	unsigned y,
	unsigned n,
	unsigned digits
)
{
	
	x += digits * 5;
	for(unsigned i = 0 ; i < digits ; i++)
	{
		if (x >= WIDTH)
			return WIDTH;
		x -= 5;
		draw_char(x, y, '0' + (n % 10));
		n /= 10;
	}

	return x + digits * 5;
}

static unsigned
draw_num_small(
	unsigned x,
	unsigned y,
	unsigned n,
	unsigned digits
)
{
	x += digits * 4;
	for(unsigned i = 0 ; i < digits ; i++)
	{
		if (x >= WIDTH)
			return WIDTH;

		x -= 4;
		draw_small_digit(x, y, (n % 10), 0xFF);
		n /= 10;
	}

	return x + digits * 4;
}

void step_pattern(void)
{
	const uint32_t col = (millis() / 100) % WIDTH;
	for(int x = 0 ; x < WIDTH ; x++)
		for(int y = 0 ; y < HEIGHT ; y++)
			ledmatrix_set(x, y, x == col);
}

static uint32_t last_sync;
static int millis_offset;

static void sync_time(void)
{
	Spark.syncTime();
	if (Time.year() < 2000)
		return;

	// millis offset is offset from now to millis since UTC+1 midnight
	
	const int hour = (Time.hour() + 1) % 24;
	const int min = Time.minute();
	const int sec = Time.second();

	last_sync = millis();
	millis_offset = 1000 * (hour * 3600 + min * 60 + sec) - last_sync;
}

void loop()
{
	//step_pattern();
	//return;

	const uint32_t now_millis = millis();
	static uint32_t last_millis;

	if (now_millis - last_sync > ONE_DAY_MILLIS || last_sync == 0)
		sync_time();

	if (now_millis > last_millis + 100)
	{
		last_millis = now_millis;

		// 012345678901234567
		// YYYYMMDD HHMMSS.
		unsigned x = 2; // pad the first few chars

		draw_clear();

		unsigned hour = Time.hour();
		unsigned min = Time.minute();
		unsigned sec = Time.second();

		// draw the HH:MM:SS with colons inbetween
		x = draw_num(x, 0, hour, 2) + 2;
		ledmatrix_set_col(x-2, 0, 0x14, 0xFF);
		x = draw_num(x, 0, min, 2) + 2;
		ledmatrix_set_col(x-2, 0, 0x14, 0xFF);
		x = draw_num(x, 0, sec, 2) + 2;

   	// (UTC+1seconds + (UTC+1minutes * 60) + (UTC+1hours * 3600)) / 86.4
	unsigned millis_since_midnight = (now_millis + millis_offset) % ONE_DAY_MILLIS;
	unsigned milli_beats = millis_since_midnight / 86.4;

	draw_num(3, 8, milli_beats / 1000, 3);
	ledmatrix_set_col(18, 8, 0x40, 0xFF);
	draw_num(20, 8, milli_beats % 1000, 3);
	//draw_num(0, 0, millis_offset, 7);
	//draw_num(0, 8, now_millis, 7);

#if 0
	//for (int i = 0 ; i < 128 ; i++)
	{
		unsigned ms = (millis() / 10) % 100;
		unsigned y = 81;
		draw_num_small(y, ms, 2);

		ledmatrix_draw();
	}
#endif
	}

	ledmatrix_draw();
}

void setup()
{
	Spark.function("show", show_string);
	ledmatrix_setup();
	//draw_string("redscroller");
	sync_time();
}

