#include "redscroller.h"

void setup()
{
	ledmatrix_setup();
}

void loop()
{
	draw_clear();
	draw_string("Hello world!@");
	ledmatrix_draw();
}
