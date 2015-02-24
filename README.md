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
Tracks the current time.  Allows you to query it and set an alarm.
When the time matches, the blue LED will turn on.

