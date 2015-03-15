#include "redscroller.h"
#define ONE_DAY_MILLIS (24*60*60*1000)

static int
show_string(String s)
{
	draw_clear();

	for(unsigned i = 0 ; i < s.length() && i*5 < 90 ; i++)
		draw_char(i*5, s.charAt(i));

	return s.length();
}

void setup()
{
	Spark.function("show", show_string);
	ledmatrix_setup();
	draw_string("redscroller");
}


static unsigned
draw_num(
	unsigned y,
	unsigned n,
	unsigned digits
)
{
	
	y += digits * 6;
	for(unsigned i = 0 ; i < digits ; i++)
	{
		if (y >= WIDTH)
			return WIDTH;
		y -= 6;
		draw_char(y, '0' + (n % 10));
		n /= 10;
	}

	return y + digits * 6;
}

static unsigned
draw_num_small(
	unsigned y,
	unsigned n,
	unsigned digits
)
{
	y += digits * 4;
	for(unsigned i = 0 ; i < digits ; i++)
	{
		if (y >= WIDTH)
			return WIDTH;

		y -= 4;
		draw_small_digit(y, (n % 10), 0xFF);
		n /= 10;
	}

	return y + digits * 4;
}

void loop()
{
	static uint32_t last_sync;
	const uint32_t now_millis = millis();
	static uint32_t last_millis;

	if (now_millis - last_sync > ONE_DAY_MILLIS)
	{
		Spark.syncTime();
		last_sync = millis();
		return;
	}

	//if (now_millis > last_millis + 900)
	{
		last_millis = now_millis;

		// 012345678901234567
		// YYYYMMDD HHMMSS.
		unsigned y = 0; // pad the first few chars

		unsigned year = Time.year();
		unsigned mon = Time.month();
		unsigned day = Time.day();

		draw_clear();

		y = draw_num_small(y, year, 4) + 2;
		ledmatrix_set_col(y-2, 0x08, 0xFF);
		y = draw_num_small(y, mon, 2) + 2;
		ledmatrix_set_col(y-2, 0x08, 0xFF);
		y = draw_num_small(y, day, 2) + 2;

		unsigned hour = Time.hour();
		unsigned min = Time.minute();
		unsigned sec = Time.second();

		// draw the HH:MM:SS with colons inbetween
		y = draw_num(y, hour, 2) + 2;
		ledmatrix_set_col(y-2, 0x14, 0xFF);
		y = draw_num(y, min, 2) + 2;
		ledmatrix_set_col(y-2, 0x14, 0xFF);
		y = draw_num(y, sec, 2) + 2;
	}

	//for (int i = 0 ; i < 128 ; i++)
	{
		unsigned ms = (millis() / 10) % 100;
		unsigned y = 80;
		draw_num_small(y, ms, 2);

		ledmatrix_draw();
	}
}
