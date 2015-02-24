/** \file
 * Demo 2:
 * Expose a function that allows the user to turn the LED on or off
 * and a variable with the current value.
 */

static const int LED = D7;
static int led_state = 0;

static int
set_led(
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
	Spark.variable("led_state", &led_state, INT);
	Spark.function("set_led", set_led);
}


void loop()
{
	// each time through the loop, get the current
	// state of the LED (on or off)
	led_state = digitalRead(LED);
}
