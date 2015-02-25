/** \file
 * Demo 4:
 * Keep track of the current time on the Spark Core and read an analog
 * value from a knob that is published as an event.
 */

static const int LED = D7;
static const int KNOB_PIN = A0;

static char time_str[32];
static int knob;


#define ONE_DAY_MILLIS (24*60*60*1000)

void setup()
{
	RGB.control(true);
	pinMode(LED, OUTPUT);
	pinMode(KNOB_PIN, INPUT);
	Spark.variable("time", &time_str, STRING);
	Spark.variable("knob", &knob, INT);
}


void loop()
{
	knob = analogRead(KNOB_PIN);
	RGB.color(0,0,knob/16);

	static uint32_t last_sync;
	static uint32_t last_update;
	const uint32_t now_millis = millis();

	if (now_millis - last_sync > ONE_DAY_MILLIS)
	{
		Spark.syncTime();
		last_sync = millis();
		return;
	}

	// Once per second update the time string and publish the
	// current knob data.
	if (now_millis - last_update > 1000)
	{
		last_update = now_millis;

		String n = Time.timeStr();
		n.getBytes((unsigned char*) time_str, sizeof(time_str));

		Spark.publish("knob", String(knob), 60, PRIVATE);
	}
}
