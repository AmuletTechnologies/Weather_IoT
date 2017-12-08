#include <AmuletLCD.h>

/**The MIT License (MIT)

Copyright (c) 2017 by Daniel Eichhorn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

See more at https://blog.squix.org
*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <JsonListener.h>
#include "WundergroundForecast.h"
#include "WundergroundConditions.h"

/**
 * Wunderground Settings
 */
const String  WUNDERGRROUND_API_KEY = "<Wunderground-KEY>";
const String  WUNDERGR_UND_STATE_OR_COUNTRY = "CA";
const String  WUNDERGR_UND_CITY = "San Jose";
const String  WUNDERGRROUND_LANGUAGE = "EN";
const boolean IS_METRIC = false;
const boolean USE_PM = true;

// initiate the WundergoundClient
WundergroundForecast wundergroundF(IS_METRIC);
WundergroundConditions wundergroundC(IS_METRIC);


/**
 * WiFi Settings
 */
const char* ESP_HOST_NAME = "esp-" + ESP.getFlashChipId();
const char* WIFI_SSID     = "SSID";
const char* WIFI_PASSWORD = "PW";

// initiate the WifiClient
WiFiClient wifiClient;

/**
 * OTA Settings
 */
#include <ESP8266mDNS.h>  // For OTA - For ESP8266... or...
#include <WiFiUdp.h>  // For OTA
#include <ArduinoOTA.h>  // For OTA
const char *OTAName = "ESP8266";           // A name and a password for the OTA service
const char *OTAPassword = "esp8266";
void otaSetup()
{
  ArduinoOTA.setHostname(OTAName);  // For OTA - Optional if you don't mind deciphering which ESP is which.
  ArduinoOTA.begin();  // For OTA
}

/**
 * Helping funtions
 */
void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println(WiFi.localIP());
  Serial.println();
}

/**
 * Amulet Settings
 */
AmuletLCD amulet;
#define S_OFFSET  99 //this allows amulet to offset its unsigned byte into a signed number, so 0-255 becomes -99-156

uint8_t dayToIndex(String dayStr){
  switch(dayStr.charAt(2)){
    case 'n':  //moNday and suNday
      if (dayStr.charAt(0) == 'S')
        return 1; //Sunday
      else
        return 2; //Monday
      break;
    case 'e':  //tuEsday
      return 3;
      break;
    case 'd':  //weDnesday
      return 4;
      break;
    case 'u':  //thUrsday
      return 5;
      break;
    case 'i':  //frIday
      return 6;
      break;
    case 't':  //saTurday
      return 7;
      break;
    default:
      return 8; //invalid day
  }
}

uint8_t iconToIndex(String iconText){
  if (iconText == "chanceflurries") return 0;
  if (iconText == "chancerain") return 1;
  if (iconText == "chancesleet") return 2;
  if (iconText == "chancesnow") return 3;
  if (iconText == "chancetstorms") return 4;
  if (iconText == "clear") return 5;
  if (iconText == "cloudy") return 6;
  if (iconText == "flurries") return 7;
  if (iconText == "fog") return 8;
  if (iconText == "hazy") return 9;
  if (iconText == "mostlycloudy") return 10;
  if (iconText == "mostlysunny") return 11;
  if (iconText == "partlycloudy") return 12;
  if (iconText == "partlysunny") return 13;
  if (iconText == "rain") return 14;
  if (iconText == "sleet") return 15;
  if (iconText == "snow") return 16;
  if (iconText == "sunny") return 17;
  if (iconText == "tstorms") return 18;
  return 19; //unknown
}


/**
 * SETUP
 */
void setup() {
  amulet.begin(115200);
  delay(1000);
  connectWifi();
  otaSetup();
  //Serial.println();
  //Serial.println("\n\nNext Loop-Step: " + String(millis()) + ":");
  uint8_t maxForecasts = 10;
  WGForecast forecasts[maxForecasts];
  wundergroundF.updateForecast(forecasts, maxForecasts, WUNDERGRROUND_API_KEY, WUNDERGRROUND_LANGUAGE, WUNDERGR_UND_STATE_OR_COUNTRY, WUNDERGR_UND_CITY);
  
  
  for (int i = 0; i < maxForecasts; i+=2) { //skip nights
    amulet.setByte(10+i,dayToIndex(forecasts[i].forecastTitle));
    amulet.setByte(11+i,iconToIndex(forecasts[i].forecastIcon));
    amulet.setByte(20+i,forecasts[i].forecastHighTemp.toInt()+S_OFFSET);
    amulet.setByte(21+i,forecasts[i].forecastLowTemp.toInt()+S_OFFSET);
    /*Serial.println("------------------------------------");
    Serial.println("getForecastIcon: " + forecasts[i].forecastIcon);
    Serial.println("getForecastTitle: " + forecasts[i].forecastTitle);
    Serial.println("getForecastLowTemp: " + forecasts[i].forecastLowTemp);
    Serial.println("getForecastHighTemp: " + forecasts[i].forecastHighTemp);
    Serial.println("getForecastDay: " + forecasts[i].forecastDay);
    Serial.println("getForecastMonth: " + forecasts[i].forecastMonth);
    Serial.println("getForecastText: " + forecasts[i].forecastText);
    Serial.println("getPoP: " + forecasts[i].PoP);*/

  }


  //Serial.println();
  //Serial.println("---------------------------------------------------/\n");

  WGConditions conditions;
  wundergroundC.updateConditions(&conditions, WUNDERGRROUND_API_KEY, WUNDERGRROUND_LANGUAGE, WUNDERGR_UND_STATE_OR_COUNTRY, WUNDERGR_UND_CITY);
  amulet.setByte(0,conditions.currentTemp.toInt()+S_OFFSET);
  
  amulet.setString(0,(conditions.weatherText + '\n' + conditions.humidity + " humidity").c_str());
  /*Serial.println("wundergroundDate: " + conditions.date);

  Serial.println("wundergroundWindSpeed: " + conditions.windSpeed);
  Serial.println("wundergroundWindDir: " + conditions.windDir);

  Serial.println("wundergroundCurrentTemp: " + conditions.currentTemp);
  Serial.println("wundergroundTodayIcon: " + conditions.weatherIcon);
  Serial.println("wundergroundTodayIconText: " + conditions.weatherText);
  Serial.println("wundergroundMeteoconIcon: " + wundergroundC.getMeteoconIcon(conditions.weatherIcon));
  Serial.println("wundergroundWeatherText: " + conditions.weatherText);
  Serial.println("wundergroundHumidity: " + conditions.humidity);
  Serial.println("wundergroundPressure: " + conditions.pressure);
  Serial.println("wundergroundDewPoint: " + conditions.dewPoint);
  Serial.println("wundergroundPrecipitationToday: " + conditions.precipitationToday);

  Serial.println();
  Serial.println("---------------------------------------------------/\n");*/

}


/**
 * LOOP
 */
void loop() {
  amulet.serialEvent();   // Handle incoming requests from Amulet module
  ArduinoOTA.handle();    // Handle OTA from Arduino IDE
}
