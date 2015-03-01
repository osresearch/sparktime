#include "sparktime.h"
#define ONE_DAY_MILLIS (24*60*60*1000)


void setup()
{
	ledmatrix_setup();
	draw_clear();
}


void loop()
{
	static uint32_t last_sync;
	const uint32_t now_millis = millis();

	if (now_millis - last_sync > ONE_DAY_MILLIS)
	{
		Spark.syncTime();
		last_sync = millis();
		return;
	}

	uint8_t hour = Time.hour();
	const uint8_t min = Time.minute();

	// Time zone update for eastern time
	hour = (hour - 5 + 24) % 24;

	draw_time(
		hour / 10,
		hour % 10,
		min / 10,
		min % 10
	);

	for (int i = 0 ; i < 10 ; i++)
	{
		ledmatrix_draw();
	}
}
