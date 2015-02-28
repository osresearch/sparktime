/** \file
 * Interface a sparkcore with the SolderTime Desk Clock
 */

#define WIDTH 20
#define HEIGHT 7
static uint8_t fb[WIDTH][HEIGHT];

void setup()
{
	pinMode(A0, OUTPUT); // col b0
	pinMode(A1, OUTPUT); // col b1
	pinMode(A2, OUTPUT); // col b2
	pinMode(A3, OUTPUT); // S0
	pinMode(A4, OUTPUT); // S1
    
	pinMode(D0, OUTPUT); // row0 .. row6
	pinMode(D1, OUTPUT);
	pinMode(D2, OUTPUT);
	pinMode(D3, OUTPUT);
	pinMode(D4, OUTPUT);
	pinMode(D5, OUTPUT);
	pinMode(D6, OUTPUT);
    
	pinMode(D7, OUTPUT); // led
}

// configure the column output, but select the no-decoder
void col_output(uint8_t col)
{
	col %= 8;
	digitalWrite(A3, 1);
	digitalWrite(A4, 1);
    
	digitalWrite(A0, col & 1);
	digitalWrite(A1, col & 2);
	digitalWrite(A2, col & 4);
}

void col_enable(uint8_t col)
{
	static const uint8_t sel[] = { 1, 2, 0 };

	col = sel[col / 8];
    
	digitalWrite(A3, col & 1);
	digitalWrite(A4, col & 2);
}

void draw()
{
	for(uint8_t col=0 ; col <= WIDTH ; col++)
	{
		col_output(col);
		const uint8_t * const b = fb[col];
		digitalWrite(D0, b[0]);
		digitalWrite(D1, b[1]);
		digitalWrite(D2, b[2]);
		digitalWrite(D3, b[3]);
		digitalWrite(D4, b[4]);
		digitalWrite(D5, b[5]);
		digitalWrite(D6, b[6]);
		col_enable(col);
		delayMicroseconds(200);
	}

	col_enable(WIDTH+1);
}

void loop()
{
	for (uint8_t col=0 ; col < WIDTH ; col++)
		for(uint8_t row=0 ; row < HEIGHT ; row++)
			fb[col][row] ^= 1;

	draw();
	digitalWrite(D7, !digitalRead(D7));
}
