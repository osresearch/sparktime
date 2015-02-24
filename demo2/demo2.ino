/** \file
 * Demo 2:
 * Expose a function that allows the user to turn the LED on or off
 * and a variable with the current value.
 */

static const int LED = D7;
static int led_state = 0;

static int
write_led(
	String command
)
{
	if (command == "0")
	{
		digitalWrite(LED, 0);
		return 0;
	} else
	if (command == "1")
	{
		digitalWrite(LED, 1);
		return 1;
	} else {
		return -1;
	}
}


void setup()
{
	pinMode(LED, OUTPUT);
	Spark.variable("read", &led_state, INT);
	Spark.function("write", write_led);
}


void loop()
{
	led_state = digitalRead(LED);
}
