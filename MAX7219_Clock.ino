/*****************************************************************
  WiFi-clock with MAX7219 8x8 matrix display.

  Based on: NTP TZ DST - bare minimum
  NetWork Time Protocol - Time Zone - Daylight Saving Time
  https://werner.rothschopf.net/202011_arduino_esp8266_ntp_en.htm
  https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/NTP-TZ-DST/NTP-TZ-DST.ino

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see https://www.gnu.org/licenses/.

  Code by Potatopasty https://tg-music.neocities.org
  potatopasty@pm.me
******************************************************************/

/* Necessary Includes */

#include <ESP8266WiFi.h>            // we need wifi to get internet access
#include <WifiUDP.h>
#include <time.h>                   // time() ctime()
#include <WiFiManager.h>            // Use this, do not hard code your wifi name and password
#include <NTPClient.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>           // https://github.com/markruys/arduino-Max72xxPanel

#include "customFont.h"

int pinCS = 2;
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays   = 1;
int intensity;
char hour_value[20];
char min_value[20];

// Display wiring:
// Vcc  -> 3v
// Gnd  -> Gnd
// DIN  -> D7
// CS   -> D4
// CLK  -> D5

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

/* Configuration of NTP */
#define MY_NTP_SERVER "europe.pool.ntp.org"      // Europe   https://www.pool.ntp.org
#define MY_TZ "EET-2EEST,M3.5.0/3,M10.5.0/4"     // Helsinki https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, MY_NTP_SERVER);

void setup() {

  configTime(MY_TZ, MY_NTP_SERVER); // --> Here is the IMPORTANT ONE LINER needed in your sketch!

  matrix.setIntensity(1);
  matrix.setRotation(0, 1);
  matrix.setRotation(1, 1);
  matrix.setRotation(2, 1);
  matrix.setRotation(3, 1);
  matrix.fillScreen(LOW);
  matrix.setFont(&customFont);
  matrix.drawChar(0, 5, 'A', HIGH, LOW, 1);
  matrix.drawChar(4, 5, 'D', HIGH, LOW, 1);
  matrix.drawChar(8, 5, 'D', HIGH, LOW, 1);
  matrix.drawChar(14, 5, 'W', HIGH, LOW, 1);
  matrix.drawChar(19, 5, 'i', HIGH, LOW, 1);
  matrix.drawChar(22, 5, '-', HIGH, LOW, 1);
  matrix.drawChar(25, 5, 'F', HIGH, LOW, 1);
  matrix.drawChar(29, 5, 'i', HIGH, LOW, 1);
  matrix.write();

  // Set your own password and network name. IP-Address: 192.168.4.1 for Wifi setup.
  WiFiManager wifiManager;
  wifiManager.autoConnect("AP-NAME", "AP-PASSWORD");

  // start network
  WiFi.begin();
  Serial.begin(9600);
  while (WiFi.status() != WL_CONNECTED) {
    matrix.fillScreen(LOW);
    matrix.setFont(&customFont);
    matrix.drawChar(6, 5, 'd', HIGH, LOW, 1);
    matrix.drawChar(11, 5, 'O', HIGH, LOW, 1);
    matrix.drawChar(16, 5, 'N', HIGH, LOW, 1);
    matrix.drawChar(22, 5, 'e', HIGH, LOW, 1);
    matrix.write();
    delay(5000);
  }
  // by default, the NTP will be started after 60 secs
}

void loop() {

  intensity = map(analogRead(0), 0, 1024, 0, 7);
  matrix.setIntensity(intensity);
  matrix.fillScreen(LOW);
  time_t now = time(nullptr);
  String time = String(ctime(&now));
  time.trim();

  time.substring(11, 13).toCharArray(hour_value, 10);
  time.substring(14, 16).toCharArray(min_value, 10);

// This mess is here to remove leading zero from hours. 

  int tunnit = atoi (hour_value);
  matrix.setFont(&customFont);

  if (tunnit < 10) {
    matrix.drawChar(1, 7, ' ', HIGH, LOW, 1);
  }
  else {
    matrix.drawChar(1, 7, hour_value[0], HIGH, LOW, 1);
  }
  matrix.drawChar(8, 7, hour_value[1], HIGH, LOW, 1);

  matrix.drawChar(14, 7, ':', HIGH, LOW, 1);

  matrix.drawChar(18, 7, min_value[0], HIGH, LOW, 1);
  matrix.drawChar(25, 7, min_value[1], HIGH, LOW, 1);
  matrix.write();

}
