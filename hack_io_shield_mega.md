This is a simple tutorial that shows how hack nueletronics IO shield to works with arduino mega.

This link helped me a lot: http://mcukits.com/2009/04/06/arduino-ethernet-shield-mega-hack/
In lib, I chose not to use the lib of nueletronics. The ds1302 Lib from official site was used and modified (available on downloads) in order to work with ds1302 IO shield pins.
For this shield works on arduino mega you will need to isolate the pins of shield: 10, 11, 12, 13 and connect with arduino mega pins: 50, 51, 52, 53.

  * MEGA pin 50 (MISO) to Arduino Shield pin 12.
  * MEGA pin 51 (MOSI) to Arduino Shield pin 11.
  * MEGA pin 52 (SCK) to Arduino Shield pin 13.
  * MEGA pin 53 (SS) to Arduino Shield pin 10.