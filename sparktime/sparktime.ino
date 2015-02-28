#include "sparktime.h"
#
void setup()
{
	ledmatrix_setup();
	draw_string("HELO");
}

void loop()
{
	ledmatrix_draw();
}
