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

void loop()
{
	static uint32_t last_sync;
	const uint32_t now_millis = millis();
	static unsigned last_second;

	if (now_millis - last_sync > ONE_DAY_MILLIS)
	{
		Spark.syncTime();
		last_sync = millis();
		return;
	}

	unsigned cur_sec = now_millis / 1000;

	if (cur_sec != last_second)
	{
		last_second = cur_sec;

		// 012345678901234567
		// YYYYMMDD HHMMSS.
		unsigned y = 0; // pad the first few chars

		unsigned year = Time.year();
		unsigned month = Time.month();
		unsigned day = Time.day();

		draw_clear();
		ledmatrix_set_col(2, 0xAA, 0xFF);
		ledmatrix_set_col(3, 0x55, 0xFF);
		ledmatrix_set_col(3, 1 << (last_second & 7), 0xFF);


		draw_small_digit(y += 4, (year / 1000) % 10, 0);
		draw_small_digit(y += 4, (year / 100) % 10, 0);
		draw_small_digit(y += 4, (year / 10) % 10, 0);
		draw_small_digit(y += 4, (year / 1) % 10, 0);

		ledmatrix_set_col(y++ + 3, 0xFF, 0xFF);
		//y++;

		draw_small_digit(y += 4, (month / 10) % 10, 0);
		draw_small_digit(y += 4, (month / 1) % 10, 0);
		y++;

		draw_small_digit(y += 4, (day / 10) % 10, 0);
		draw_small_digit(y += 4, (day / 1) % 10, 0);
		y++;

		unsigned hour = Time.hour();
		unsigned min = Time.minute();
		unsigned sec = Time.second();

		draw_small_digit(y += 4, (hour / 10) % 10, 0);
		draw_small_digit(y += 4, (hour / 1) % 10, 0);
		y++;
		draw_small_digit(y += 4, (min / 10) % 10, 0);
		draw_small_digit(y += 4, (min / 1) % 10, 0);
		y++;
		draw_small_digit(y += 4, (sec / 10) % 10, 0);
		draw_small_digit(y += 4, (sec / 1) % 10, 0);
		y++;
	}

	for (int i = 0 ; i < 128 ; i++)
	{
		unsigned ms = millis() % 1000;
		unsigned y = 62;
		draw_small_digit(y += 4, (ms / 100) % 10, 0);
		draw_small_digit(y += 4, (ms / 10) % 10, 0);
		draw_small_digit(y += 4, (ms / 1) % 10, 0);

		ledmatrix_draw();
	}
}
