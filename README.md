Useful commands
===


Demo 1
---
Just flashes the blue LED on the board at 1 Hz.  Find out your core name
with the `list` command and then call the flash command on that name:

    spark list
    spark flash CORE_NAME demo1


Demo 2
---
Exposes a function to allow a remote process to turn the LED on or off.
Also has a variable to allow a remote read of the LED state. Flash it:

    spark flash CORE_NAME demo1

And then use the command line utility to turn the LED on, to check
the state, turn it off, as well as see how it handles invalid input:

    spark call CORE_NAME write 1
    spark get CORE_NAME read
    spark call CORE_NAME write 0
    spark call CORE_NAME write blah

You can also use curl to do this query, although you will need two pieces
of information: your private access token and your core ID.

![Device ID](images/core-id.png)
For the access token, click on the Gear Box icon and copy the token.

![Access Token](images/access-token.png)
For the device id, select the Cores icon (looks like a target reticle)
and then click the dropdown next to the core name to get the id.

With these two pieces of info, you can use `curl` to turn on the LED by
doing an HTTP `POST` with the access token and argument as form elements:

    DEVICE_ID=(paste the hex device id token...)
    ACCESS_TOKEN=(paste the hex access token...)
    curl "https://api.spark.io/v1/devices/$DEVICE_ID/write" \
	-d access_token=$ACCESS_TOKEN \
	-d args="1"

    curl "https://api.spark.io/v1/devices/$DEVICE_ID/write" \
	-d access_token=$ACCESS_TOKEN \
	-d args="0"

You can read the value of the variable by doing an HTTP `GET`, passing
in the access token as a query parameter.

    curl "https://api.spark.io/v1/devices/$DEVICE_ID/read?access_token=$ACCESS_TOKEN"


Demo 4
---
Tracks the current time.  Allows you to query it and set an alarm.
When the time matches, the blue LED will turn on.

