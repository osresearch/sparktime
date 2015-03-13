void setup()
{
	pinMode(D0, INPUT);
	pinMode(D1, INPUT);
	pinMode(D2, INPUT);
	pinMode(D3, INPUT);
	pinMode(D4, INPUT);
	pinMode(D5, INPUT);
	pinMode(D6, INPUT);

	digitalWrite(D0, 0);
	digitalWrite(D1, 0);
	digitalWrite(D2, 0);
	digitalWrite(D3, 0);
	digitalWrite(D4, 0);
	digitalWrite(D5, 0);
	digitalWrite(D6, 0);
}

void loop()
{
	pinMode(D0, OUTPUT);
	pinMode(D0, INPUT);

	pinMode(D1, OUTPUT);
	pinMode(D1, INPUT);

	pinMode(D2, OUTPUT);
	pinMode(D2, INPUT);

	pinMode(D3, OUTPUT);
	pinMode(D3, INPUT);

	pinMode(D4, OUTPUT);
	pinMode(D4, INPUT);

	pinMode(D5, OUTPUT);
	pinMode(D5, INPUT);

	pinMode(D6, OUTPUT);
	pinMode(D6, INPUT);
}
