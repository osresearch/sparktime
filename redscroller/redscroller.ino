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
}

static inline void
row(
	const int pin
)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, 0);
	delay(100);
	pinMode(pin, INPUT);
}


void loop()
{
	row(D0);
	row(D1);
	row(D3);
	row(D4);
	row(D5);
	row(D6);
	row(D7);
}
