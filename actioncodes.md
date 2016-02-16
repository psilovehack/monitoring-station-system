<b>Action code</b> is a integer number that allows communication between arduino board and host pc.

In the <b>arduino</b> code has two functions that separates the action value if the action comes with a value.

```
if (action.indexOf(ACTION_VALUE_SEPARATOR) == -1) {
 executeAction(action.toInt());
} 
else {
 executeAction(getActionId(action), getActionValue(action));
}
```

The project in actual version([r19](https://code.google.com/p/monitoring-station-system/source/detail?r=19)) support the following action codes:

<b>Activity sensor:</b>

  * READ\_ACTIVITY = 350 (not need value)
    1. This action read and return 1 if activity was detected through the infrared sensor.
    1. Returns `350=1` (if auto-notifying is enabled) case activity was detected.
  * ACTIVITY\_NOTIFY\_CHANGE = 351 (0 or 1)
    1. Changes the flag to auto-notifying via serial that activity was detected.
  * ACTIVITY\_NOTIFY\_STATUS = 352 (not need value)
    1. Value of auto-notifying status.

<b>Temp. and humidity sensor:</b>
  * READ\_TEMP = 360 (not need value)
    1. Returns the temperature (in oC)
  * READ\_HUMIDITY = 370 (not need value)
    1. Reads the humidity (%)

<b>System actions:</b>
  * RESET\_BOARD = 100 (not need value)
    1. Resets the software
  * FREE\_MEMORY = 100 (not need value)
    1. Executes malloc function and return the free memory available after operation
  * UP\_TIME = 120 (not need value)
    1. Returns system startup time (same as millis() arduino function)

  * LOG = 210
    1. Not implemented yet. Only prints the value.