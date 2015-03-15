/** \file
 * Demo 2:
 * Expose a function that allows the user to turn the LED on or off
 * and a variable with the current value.
 */

static const int LED = D7;
static int led_state = 0;

// set_led() both sets the LED, and returns what it has been set to.
static int
set_led(
	String command
)
{
	if (command == "0")
	{
		// digitalWrite() and digitalRead() are defined in the 
		// SparkCore docs on their site
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


/*
 * Variables and functions can be "exposed" through the Spark Cloud
 * so that you can call them with GET:
 * 	GET /v1/devices/{DEVICE_ID}/{VARIABLE}
 * and POST:
 * 	POST device/{FUNCTION}
 */
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
