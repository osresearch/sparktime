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

And then use the command line utility to turn the LED on and off,
as well as see how it handles invalid input:

    spark call CORE_NAME write 1
    spark call CORE_NAME write 0
    spark call CORE_NAME write blah

Demo 4
---
Tracks the current time.  Allows you to query it and set an alarm.
When the time matches, the blue LED will turn on.

