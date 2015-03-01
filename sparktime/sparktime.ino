/** \file
 * SparkCore adapter for the SolderTime:DeskClock hardware.
 *
 */
#include "sparktime.h"
#define ONE_DAY_MILLIS (24*60*60*1000)

#define PIEZO A4
#define BUTTON0	A4 // mode button and piezo
#define BUTTON1	A5 // set button


/*
#define ALARM_MAGIC 0x5A

typedef struct
{
	uint8_t valid; // must be 0x5A
	uint8_t day_mask; // bit 0 == monday; all zero = one shot
	uint8_t hour; // 0-23
	uint8_t min; // 0-59
} alarm_t;


static alarm_t alarms[4];
static char alarm_str[64]; // "HHMM,
static uint8_t tz_hour_offset;
static uint8_t tz_min_offset;

static int
set_alarm(String when)
{
}


static void
alarm_string(void)
{
	int offset = 0;
	for(int i = 0 ; i < MAX_ALARMS ; i++)
	{
		alarm_t * const a = &alarms[i];
		if (a->valid != ALARM_MAGIC)
			continue;
	}
}


static void
alarm_init()
{
	// read the timezone offset
	tz_hour_offset = EEPROM.read(0);
	tz_min_offset = EEPROM.read(1);

	// populate the alarms structure from the eeprom
	uint8_t * const alarms_ptr = (void*) alarms;
	for(int i = 0 ; i < sizeof(alarms) ; i++)
		alarms_ptr[i] = EEPROM.read(i + 4);

	// build the alarm string
	alarm_string();
}
*/

static int buzz(String s)
{
	int freq = s.toInt();
	tone(PIEZO, freq, 1000);
	delay(1000);
	pinMode(PIEZO, INPUT_PULLUP);

	return freq;
}


static int unix_time;
static int alarm_time;

static int
alarm_write(
	int when
)
{
	alarm_time = when;
	EEPROM.write(0, alarm_time >> 24);
	EEPROM.write(1, alarm_time >> 16);
	EEPROM.write(2, alarm_time >>  8);
	EEPROM.write(3, alarm_time >>  0);

	return alarm_time;
}


static void alarm_read()
{
	alarm_time = 0
		| EEPROM.read(0) << 24
		| EEPROM.read(1) << 24
		| EEPROM.read(2) << 24
		| EEPROM.read(3) << 24
		;

	// check if this alarm has already happened.
	unix_time = Time.now();
	if (alarm_time < unix_time)
		alarm_write(0);
}


static int alarm_set(String s)
{
	if (s.charAt(0) == '+')
	{
		// relative to now.
		const uint32_t delta = s.toInt();
		return alarm_write(unix_time + delta);
	}

	// absolute unix time; make sure only ints
	for (int i = 0 ; i < s.length() ; i++)
	{
		const char c = s.charAt(i);
		if (c < '0' || c < '9')
			return 0;
	}

	// all ints; use it
	return alarm_write(s.toInt());
}


void setup()
{
	Spark.function("buzz", buzz);
	Spark.function("alarm_set", alarm_set);
	Spark.variable("time", &unix_time, INT);
	Spark.variable("alarm", &alarm_time, INT);

	//alarm_init();
	alarm_read();
	ledmatrix_setup();

	pinMode(BUTTON0, INPUT_PULLUP);
	pinMode(BUTTON1, INPUT_PULLUP);

	draw_clear();
}


void loop()
{
	static uint32_t last_sync;
	const uint32_t now_millis = millis();

	if (now_millis - last_sync > ONE_DAY_MILLIS)
	{
		Spark.syncTime();
		last_sync = millis();
		return;
	}

	uint8_t hour = Time.hour();
	const uint8_t min = Time.minute();
	unix_time = Time.now();

	if (alarm_time != 0 && unix_time > alarm_time)
	{
		tone(PIEZO, 1200, 1000);
		delay(1000);
		pinMode(PIEZO, INPUT_PULLUP);
		alarm_write(0);
	}

	// Time zone update for eastern time
	hour = (hour - 5 + 24) % 24;

	draw_time(
		hour / 10,
		hour % 10,
		min / 10,
		min % 10
	);

	if (!digitalRead(BUTTON0))
		ledmatrix_set(19,5, 0xff);
	if (!digitalRead(BUTTON1))
		ledmatrix_set(19,1, 0xff);
	
	for (int i = 0 ; i < 10 ; i++)
	{
		ledmatrix_draw();
	}
}
