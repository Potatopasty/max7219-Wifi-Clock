# MAX7219 Wifi Clock

This is a WiFi-clock with automatic daylight saving time.

You don't need any buttons to set time on this clock. Potentiometer is for brightness and button is for reset.

Display can be almost any type. This time I used MAX7219 without blinking center colon. I don't like blinking clocks.

Tested with NodeMCU ESP8266 and Wemos D1 Mini.

Based on: NTP TZ DST - bare minimum
NetWork Time Protocol - Time Zone - Daylight Saving Time
https://werner.rothschopf.net/202011_arduino_esp8266_ntp_en.htm
https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/NTP-TZ-DST/NTP-TZ-DST.ino
