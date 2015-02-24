/** \file
 * Demo 1:
 * Blink the onboard LED.
 */

static const int LED = D7;

void setup()
{
	pinMode(LED, OUTPUT);
}


void loop()
{
	const uint32_t now = millis();
	static uint32_t last_blink;
	const uint32_t on_time = 500;

	if (now - last_blink > 500)
	{
		last_blink = now;
		digitalWrite(LED, !digitalRead(LED));
	}
}
