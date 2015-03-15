#include "redscroller.h"

static int
show_string(String s)
{
	draw_clear();

	for(int i = 0 ; i < s.length() && i*5 < 90 ; i++)
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
	ledmatrix_draw();
}
