# Weather_IoT
Arduino code to pull weather data from Weather Underground site

1) ESP8266 Board profile (skip to #2 if you already have this)

Install the ESP8266 Board package through the Arduino Boards Manager. 
    First make sure the Boards Manager can see the Espressif boards by going to:
        File > Preferences
    Then add this to the "Additional Board Manager URLs"
        http://arduino.esp8266.com/stable/package_esp8266com_index.json
    Then hit okay and go to:
        Tools > Board > Boards Manager...
    Type in "esp8266", without quotes, into the search filter and install the package titled "esp8266 by ESP8266 Community"


2) Libraries

Install the following library by downloading from github and extracting into your Arduino Sketchbook Libraries

http://github.com/AmuletTechnologies/AmuletLCD

Install the following libraries with your Arduino Library Manager in Sketch > Include Library > Manage Libraries...

    ESP8266 Weather Station

    Json Streaming Parser (by Daniel Eichhorn)

Go to http://wunderground.com, create an account and get an API Key

    In the Arduino IDE go to File > Open... and navigate to this folder and open the WundergroundToAmulet project

    Enter the Wunderground API Key

    Enter your Wifi credentials

    Adjust the location according to Wunderground API, e.g. Zurich, CH  or San_Jose CA

    Adjust UTC offset





