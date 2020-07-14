# <div align="center">Dawn Alarm Clock Documentation</div>

<br>
<br>

For our project we decided to implement a Dawn simulator alarm clock. Simply put, it has all of the functionality of an alarm clock with a few bonus features. Some of those features worth noting are; 

- LEDS that begin simulating dawn 5 minutes before the alarm (inc. 8 customizable colours)
- An IR sensor giving the option to snooze the alarm with movement
- Sophisticated power management to save battery life
- Extremely user friendly interface
- An alarm that can be set up to 7 days into the future
- Accurate time management
- Local temperature and weather information
- Landscape mode!
- OTA firmware updates


<br>

# <div align="center">Overview of features</div>
Before exploring the technicalities, presented below is a general overview of the Dawn Alarm Clock and it's features. 

### Home Display
------
<br>
<div align = "center">
  <img src="images/updatedSketch.png" width="500"/>
</div>
<br>

1. **Greeting message** – A message depending on what time of day it currently is will be displayed at the top of the screen. There are four possible greetings:
    - Good morning (6am to 11.59am)
    - Good afternoon (12pm to 4.59pm) 
    - Good afternoon (5pm to 7:59pm)
    - Good night (8pm to 5:59am)<br><br>

2. **Digital time display** – This area will display the current time in HH:MM:SS, updated every second.

3. **Date display** – The bottom corner will display the current date and day of the week, updated every 24 hours.

4. **Time to alarm** – The time to alarm will be displayed under the digital time if an alarm has been set. If the time to alarm is greater than a minute it will be formatted in H:M, otherwise a countdown in seconds from a minute will commence. If no alarm has been set, or an alarm has passed, a 'No alarm set' message is shown. 

5. **Temperature** – Displays local temperature in °C and changes colour depending on the value:
    - Less than 0°C ~ White
    - Between 0 and 9.9 ~ Cyan
    - Between 10 and 19.9 ~ Green
    - Between 20 and 29.9 ~ Orange 
    - Over 30 ~ Red<br><br>

6. **Weather description** – Short explanitory text describing the mood of weather

7. **Wind speed** – Local wind speed in meters per second 

8. **Humidity** – Local humidity %

<br>
<div align = "center">
  <img src="images/newDisplay.jpg" width="500"/>
</div>
<br>

### Menu
------
<br>
<div align = "center">
  <img src="images/menu_sketch.png" width="500"/>
</div>
<br>

1. **Page title**

2. **Menu item** – Selecting a menu item will direct the user to that page.

3. **Arrow icon** – Selecting a menu arrow icon will also direct the user to that page.

<br>
<div align = "center">
  <img src="images/menu_page.jpg" width="500"/>
</div>
<br>

### Setting an Alarm
------
<br>
<div align = "center">
  <img src="images/set_alarm_sketchX.png" width="500"/>
</div>
<br>

1. **Page title**
2. **Menu icon** – Selecting this icon will direct the user to the Menu page.

3. **Alarm hours** – Displays alarm hour time.

4. **Alarm minutes** – Displays alarm minute time.

5. **Increment/decrement alarm hour** – Selecting the triangle above the hour time will increment the alarm hour, and selecting the triangle below the hour time will decrement the alarm hour.

6. **Increment/decrement alarm minute** – Selecting the triangle above the minute time will increment the alarm minute, and selecting the triangle below the minute time will decrement the alarm minute.

7. **Alarm day** – Displays the day of the alarm.

8. **Increment/decrement alarm day** – Selecting the triangle to the right of the alarm day will increment to the next day of the week, and selecting the triangle to the left of the alarm day will decrement to the previous day of the week.

9. **Set alarm** – Selecting the 'SET' button will create an alarm based on the time inputted by the user. If the alarm is set 'before' the current time, it will assume it is meant for the future (for instance - given the example in the diagram, setting the alarm for Monday at 20:08 will create an alarm for 144 hours time).

10. **Clear alarm** – Selecting the 'CLR' button will wipe the current alarm 

<br>
<div align = "center">
  <img src="images/setAlarmX.jpg" width="500"/>
</div>
<br>

### Setting a Dawn Color
------
<br>
<div align = "center">
  <img src="images/dawn_sketch.png" width="500"/>
</div>
<br>

1. **Page title**
2. **Menu icon** – Selecting this icon will direct the user to the Menu page.

3. **Colour palette** – This area will display a range of colours to choose from - each box will contain a different colour that the user can pick for the dawn simulation.

<br>
<div align = "center">
  <img src="images/set_dawn_page.jpg" width="500"/>
</div>
<br>

### Buttons
------
<br>
<div align = "center">
  <img src="images/button_sketch.png" width="500"/>
</div>
<br>

1. **Snooze** – This button can be pressed during the alarm to snooze it for 5 minutes (similar functionality to the IR sensor).

2. **Sleep** – This button can be pressed to put the unPhone into deep sleep mode and save power.

3. **Multifunctional** – This button has two purposes; It can be pressed during the alarm to stop it completely, and it can wake the unPhone from a deep sleep.

-----
<br>

### Result
-----
<br>
<div align = "center">
  <img src="images/newDisplay2.jpg" width="500"/>
</div>
<br>
<br>
<div align = "center">
  <img src="images/back_diag.jpg" width="500"/>
</div>
<br>


# <div align="center">Technical Documentation</div>
The following section will elaborate more on the technical features of the Dawn Alarm Clock, as well as describing how challenging problems discovered during development were overcome.

### 1) Time Management
----

Any alarm clock relies wholeheatedly on accurate time keeping, without which, its uses become entirely redundant. For this reason, implementing strictly correct time mangement was of upmost priority during this project.

Early implementations involved setting the time manually and then using the `internal RTC` on the ESP (in this case a 150kHz oscillator) to keep track of it. It managed to do a reasonable job, yet due to the fact that all clocks drift, it wasn't long before the time shown was incorrect. After some testing we recorded the following results;

| Time Start (H:M:S) | Time End | Time recorded | Length  | Drift (Seconds) |
|--------------------|----------|---------------|---------|-----------------|
| 17:42:00           | 18:42:00 | 18:43:30      | 1 hour  | 90s             |
| 16:40:01           | 17:40:01 | 16:41:26      | 1 hour  | 85s             |
| 14:30:30           | 15:30:30 | 15:31:58      | 1 hour  | 88s             |
| 12:24:35           | 14:24:35 | 14:27:44      | 2 hours | 189s            |
| 15:34:02           | 16:34:02 | 16:38:40      | 3 hours | 278s            |
*Avg drift p/hour = 27.7s*

Although this early implementation provided a way of tracking the time, it was clearly not an approriate solution by itself to be used for telling the time. For one, there was no way of resetting the time except from manually burning new firmware. This meant the drift would continue to make the time more and more inaccurate. Also, if the ESP was reset or the power was lost, the memory allocated to holding the time would be wiped. These problems prompted us to implement a function `fetchTime()` which would semi-frequently automatically syncronize the time to an atomic clock using a `Network Time Protocol (NTP)`. This solution would ensure that an accurate time could be frequently obtained, avoiding the issue of an inaccurate clock. Also, if the device was ever reset, then the time could be reset automatically.

To do this, we firstly needed to provision the ESP so that we could connect it to a network in the first place. After this we would need the ESP to send a request packet to a `NTP server`, which in turn, would respond with a time stamp packet. This information could then be used to set the internal time of the ESP with pinpoint precision. See code below;


```c++
void fetchTime() {
  //provisioning...
  joinmeManageWiFi(apSSID.c_str(), apPassword.c_str());

  //GET current time and print to serial line
  while( year < 118) { // if time returned is a year <2018 then incorrect so fetch again
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); //gets current time
    delay(1000);
    time(&time_now);
    struct tm yearCheck; //store time in tm struct
    yearCheck = *localtime(&time_now);
    //stores year - (if < 2018 then time was not retrievied correctly)
    //default is 1970...
    year = yearCheck.tm_year;
    hourFetched = yearCheck.tm_hour; //record the hour last time fetched
  }

  //disconnect WiFi/stop AP as it's no longer needed (SAVE BATTERY)
  WiFi.disconnect(true);
}
```
*The while loop ensures the correct time is obtained, as during testing we noticed it took a few attempts to recieve the packet.*

A statement must be made about the frequency of these syncronizations - too many and the drain on battery power due to connectivity would introduce a new issue. We decided that this function would be called every time the device is initially booted (so also after every reset/power outage), and in the instance these events do not occur, every two hours. This limited the inaccuracy of our time to.. 27.7(avg. drift time after 1 hour) x (2) = 55.4s. It should be noted that the device doesnt automatically turn itself on to re-syncronise. Instead, the next time it is booted (either for an alarm or manually) it will check if it has been over 2 hours and then re-syncronize. 

Although the 55.4s drift above wasnt catastrophic, there was still room for improvement. Due to our decisions with power management (see power management below), the ESP spends the majourity of its time in a `deep sleep`. Unfourtunately, this presented us with a further issue - keeping track of the time during deep sleeps. As stated the inbuilt RTC lacks the necessary accuracy - which only gets worse during sleep cycles. Clearly an `NTP server` could be used on every boot, yet, as mentioned, this would use unnecessary amounts of power. The solution to this isuse was to implement our own time counting function (just during deep sleeps), apply an offset and then reset the internal time manually;
##### Before deep sleep
1) Measure # ticks on RTC 
2) Measure epoch time
##### Once woken up 
3) Calculate number of ticks during sleep
4) Convert to seconds
5) Apply offset (just under 1%)
5) Increment epoch time by this amount
6) Reset internal time

*Note: Values from points 1/2 are stored in the RTC memory using RTC_DATA_ATTR and then loaded into RTC fast memory. This enables the information to be saved during deep sleeps.*

See the implementation below;
```c++
void updateTime() {
  //timeNow = RTC clock now - offset ...(essentially RTC clock now)
  //offset = duration of setup() during initial boot
  timeNow = rtc_time_slowclk_to_us(rtc_time_get(), esp_clk_slowclk_cal_get()) - offset_time; //this offset is from fetchTime() method
  //timeDiff = timeESP has been asleep for
  //sleepTime is recorded just before ESP goes to sleep - see inActiveSleep()
  timeDiff = timeNow - sleepTime;

  //Must convert this into seconds to add to clock
  //#secs
  int seconds = floor((timeDiff / 1000000));
  seconds = seconds * (1- (seconds * 0.00000214)); //apply calculated offset (the longer it sleeps the larger the offset)

  //#milis
  int milis = floor(timeDiff % 1000000);

  if (milis+old_milis > 1000) {
    //if milis more than 1000 add second to time
    seconds += 1;
    //make up the difference to save the rest of the milis which didnt make the 1 sec
    old_milis = (old_milis + milis) -1000 ;
  } else {
    old_milis += milis;
  }

  //Finally....
  //add the time ESP has been asleep to clock
  time_now = time_t(time_now) + seconds;
  struct timeval newTime = {.tv_sec = time_now};
  struct timezone timeZon = {.tz_minuteswest = 0 };     /* minutes west of Greenwich */

  settimeofday(&newTime, &timeZon );
  
  time(&time_now);
  timeinfo = localtime (&time_now);
  Serial.printf ("%s\n", asctime(timeinfo));
}
```

A particular note should be made about how we ended up with this formula for offset. In summary it calculates a proportional offset multiplier depending on how long the ESP has been asleep for. 

    seconds = seconds * (1- (seconds * 0.00000214)); //apply calculated offset (the longer it sleeps the larger the offset)

Due to a 27.7s avg. drift over an hour (see above), the amount to be shaved off every second = 27.7/3600 = 0.00769. However, because the drift is not linear, deducting this from every second would also lead to inaccurate results. By dividing through again i.e 0.00769/3600, you arrive at a result of 0.00000214. This value is then directly multiplied by the number of seconds the device had been asleep and subtracted from 1. By multiplying this by the time it has been asleep for again, you have applied a proportional offset to the sleep  time. 

E.g
- if slept for 10 minutes (600s), 0.8s will be shaved off the time
- if slept for 50 minutes (3000s), 19.26s will be shaved off.

### Testing to show improvement

| Time Start (H:M:S) | Time End | Time recorded | Length      | Drift (Seconds) |
|--------------------|----------|---------------|-------------|-----------------|
| 9:04:00            | 10:04:00 | 10:04:05      | 1 hour      | 5s              |
| 10:10:00           | 11:10:00 | 11:11:04      | 1 hour      | 4s              |
| 12:05:30           | 13:05:30 | 13:05:37      | 1 hour      | 7s              |
| 13:10:02           | 15:09:30 | 15:09:42      | Just<2hours | 12s             |
*Avg drift p/hour: 5.3s...522% improvement!*

This improvement influenced us to extend the two hour checking period to every 5 hours. A 26.5 second avg drift (5.3*5) didnt seem like a bad price to pay for extended battery life. 

### 2) Weather API
----------
Temperature, weather description, wind speed and humidity shown on the Home Display are pulled from a HTML request made to [Open Weather API](https://openweathermap.org/api). After a free subscription to their API weather service, we could call current weather data for one location using a generated key that came with the subscription: 

<br>
<div align = "center">
  <img src="images/API.png" width="250"/>
</div>
<br>

A request can be made in various ways such as by city name or geographic coordinates, but we chose by city ID so that the API responds with exact results which had to be looked up in a `city.list.json.gz` file provided by OpenWeather. Using the domain name, city ID and the generated key we can construct the URL for the HTTP request:

```c
String URL = PSTR("http://api.openweathermap.org/data/2.5/weather?id=") + cityId + F("&APPID=") + owKey;
```

Once the HTTP request returns a valid response code, we can extract the information we need from the API. OpenWeather uses JSON format by default, so `DynamicJsonBuffer` was used to read and extract the data we wanted:

```c
if (respCode == HTTP_CODE_OK) {
      payload = http.getString();
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload);

      if (root.success()) {
        String name = root["name"];
        JsonObject& main = root["main"];
        temperature = main["temp"];             // pull temperature value
        temperature = temperature - 273.15f;    // convert from kelvin
        humidity = main["humidity"];            // pull humidity value

        JsonObject& weather = root["weather"][0];
        String des = weather["description"];    // pull description

        JsonObject& wind = root["wind"];
        speed = wind["speed"];                  // pull wind speed value
...
```

##### Flash Memory
A peculiar issue we ran into when attempting to save the weather data in memory (so that  information can be retrieved and displayed without establishing an internet connection) was that the RTC memory was unable to retain strings or char arrays after wakeup, thus the weather description had to be stored in some other way - **flash** memory. To read and write from the ESP32 flash memory, we need to use the EEPROM library that allows us to write data in specifc memory locations:
```c
#include "EEPROM.h"
...
#define EEPROM_SIZE 64 // size in bytes ~1 byte per char
...
EEPROM.begin(EEPROM_SIZE); // initialize EEPROM with predefined size
...
char arrayToStore[50];
String des = weather["description"]; // pull weather description 
des.toCharArray(arrayToStore, des.length()+1); // convert description string to char array
EEPROM.put(0, arrayToStore); // save data in memory address
EEPROM.commit();
```
`arrayToStore` is then used in the Home Display class as an `extern` variable where the string for weather description is fetched from flash memory `EEPROM.get(0, arrayToStore)`. A limitation of saving data in flash memory however, is that there is a limited number of times it can be written to. Most devices are designed for around [100,000 to 1,000,000](https://randomnerdtutorials.com/esp32-flash-memory/) write operations, but can be read an unlimited number times - even with the lower bound of write operations this should not be an issue as the data will only be stored when the time is resynchronised and an API request is made during a momentary internet connection.

##### Security

The get request is secure as it utilizes the secure transfer protocol known as HTTPS. This is secure for two reasons. One, it ensures the validity of the website you are connected with by checking the validity of the certificate from the server. In the context of the existing system, this means that the firmware will always be downloaded from the legitimate repository on Gitlab preventing the risk of an attacker transferring malicious code to the ESP32.

Secondly, HTTPS encrypts all communications between the client and the server. Again, in this context this removes the potential for an attacker to eavesdrop on the data transferred which could contain sensitive information such as network credentials.

### 3) The Alarm
----------

With accurate time management already in place, implementing alarm functionality wasn't too technical. Most of the troubles came when coding the UI page, calculating the time until an alarm.

To calculate the time to an alarm, the function `calcTime2Alarm` in the class `AlarmUIElement.cpp` extracts the information entered by the user and then compares this with the current time, returning a value for seconds to alarm. This is then added to the value of the current time in seconds (Epoch time) to generate a future time stamp.; 

```c++
  if (the_day == currentDay && hours >= tmHour && mins >= tmMin) { //if alarm today and both hours and mins are > timenow ...
    addedSeconds = (((hours-tmHour)*60*60) + ((mins-tmMin))*60);

  } else if (the_day == currentDay && hours >= tmHour && mins < tmMin) {
    addedSeconds = ((hours-tmHour-1)*60*60) + ((60-tmMin+mins)*60);

  } else { //if alarm is for today and time < time now, then must be set for 7 days from now
    if (the_day == currentDay) {
      counter = 7;
    }
    while(the_day!=currentDay) { //if alarm not for today then iterate until we find day
      currentDay = getNextDay(currentDay);
      counter ++; //amount of days away...
    }

    if (hours >= tmHour && mins >= tmMin) { // hours and mins toggled are more than current hours and mins .. ect.ect
      //the actual calculation to determine time in seconds
      addedSeconds = (counter*24*60*60 ) +  ((hours-tmHour)*60*60) + ((mins-tmMin)*60);

    } else if (hours >= tmHour && mins < tmMin) {
      addedSeconds = (counter*24*60*60) + ((hours-tmHour-1)*60*60) + (((60-(tmMin-mins)))*60);

    } else if (hours < tmHour && mins >= tmMin) {
      addedSeconds = ((counter-1)*24*60*60) + ((24-(tmHour-hours))*60*60) + ((mins-tmMin) *60);

    } else  { //(hours < tmHour && mins < tmMin )
      addedSeconds = (counter-1)*24*60*60 + ((23-(tmHour-hours))*60*60) + (60-(tmMin-mins)) *60;
    }
  }
  alarm_time = time_t(time_now) + addedSeconds; //set global alarm_time variable
  dawn_time = alarm_time - 240; //dawn time is 4 minutes from alarm time - set global
  alarmTime = localtime(&alarm_time);
}
```
Alarm_time and dawn_time are then iteraitvly checked against time_now using: `difftime(alarm_time,time_now)` in the `loop()` function in order to check whether or not it is time to start the dawn/alarm.

Below is an overview of the flow of control behind an alarm (situated in `loop()`) which uses the time stamps calculated above;

```c++
if (alarm_exist) {
  if (time2Dawn() == 0.00 ) {
    //start dawn simulator
    if (micros() - timer >= (fade_time/255)) { //proportion fade time in
      fadePixels();
      timer = micros();
    }
    if (time2Alarm() == 0.00 ) {
      //start alarm (vibe motor)
      alarm_on = true;
      vibrate();
      uiCont->run();
      //Any motion measured from IR snoozes for 5 mins
      if (digitalRead(PIR_DOUT) == 1 ){
        touchTimer = millis(); //keep screen on for another 30 secs
        snoozeAlarm();
      }
      //button3 snoozes for 5 mins
      if (unPhone::button3()) {
        touchTimer = millis(); //keep screen on for another 30 secs
        snoozeAlarm();
      }
      //button 1 cancels alarm
      if (unPhone::button1()) {
        touchTimer = millis(); //keep screen on for another 30 secs
        stopAlarm();
      }
    }
  }
}
```

##### Testing

To test the time to alarm calculations, we setup test cases for all of the logical variants in `AlarmUIElement.cpp` . I.e An alarm for the same day but the hours were less than the current hours or an alarm for the same day but the minutes were more than the current minutes. We then output the calculated number of seconds to the terminal along with the current test information.

Below you can see the exact printout to the terminal:

    The test time now is: Wednesday, 5:20
    
    Test 1
    Logical case 1: Same day, hours >, mins >
    Alarm time set for: Wednesday, 6:21
    Seconds to Alarm:
    3660
    
    
    Test 2
    Logical case 2: Same day, hours >, mins <
    Alarm time set for: Wednesday, 6:19
    Seconds to Alarm:
    3540
    
    
    Test 3
    Logical case 3: Same day, hours <, mins >
    Alarm time set for: Wednesday, 4:21
    Seconds to Alarm:
    601260
    
    
    Test 4
    Logical case 4: Same day, hours <, mins <
    Alarm time set for: Wednesday, 4:19
    Seconds to Alarm:
    601140
    
    
    Test 5
    Logical case 1: Different day, hours >, mins >
    Alarm time set for: Friday, 6:21
    Seconds to Alarm:
    435660
    
    
    Test 6
    Logical case 2: Different day, hours >, mins <
    Alarm time set for: Saturday, 6:19
    Seconds to Alarm:
    349140
    
    
    Test 7
    Logical case 3: Different day, hours <, mins >
    Alarm time set for: Monday, 4:21
    Seconds to Alarm:
    169260
    
    
    Test 8
    Logical case 4: Different day, hours < , mins <
    Alarm time set for: Tuesday, 4:19
    Seconds to Alarm:
    82740
    
    
    Test 9
    Logical case 1: Right now
    Alarm time set for: Wednesday, 5:20
    Seconds to Alarm:
    0

Using the website: https://www.timeanddate.com/date/durationresult.html?d1=12&m1=2&y1=2020&d2=12&m2=2&y2=2020&h1=05&i1=20&s1=00&h2=06&i2=21&s2=00 you can then calculate the difference between two times. 

After checking our results we discovered all of them were correct.

*Note: To replicte the tests, set the boolean `testingCalc` to true in `AlarmUIElement.cpp`. When you set an alarm, the tests will be printed to console.* 
<br>

### 4) NeoPixel Dawn Simulator 
------
Immediately after setting up the NeoPixel libraries and testing the functionality, we came across a subtle but evident ghosting problem with the LEDs. Despite explicitly setting all the pixels to OFF (equivalent to setting their RGB values to 0,0,0), there would be times that seemingly arbitrary pixels would appear in random colours. Cutting the power being supplied from the battery entirely `unPhone::setShipping(true)` would not change the outcome either. 

The NeoPixel LED module was souldered onto breakway header connectors over the unPhone expander which allowed for a semi-permanent installation. This proved to be a practical design when attempting to fix the ghosting issue of the pixels, as perhaps it was due to the `NEOPIXEL_PORT` pin that was initially defined for the LEDs. However, disabling the physical pin on the Adafruit NeoPixel and defining a new pin did not solve the issue.

Nevertheless, the main concern was with the first pixel that was consistently not behaving as intended that we did find a solution for. We discovered that explicitly setting the first pixel to OFF followed by a short delay would prevent it from ghosting. Moreover, setting a delay between the state of the pixels and displaying them appeared to ameliorate the ghosting issue as a whole:   

```c
void pixelsOff() {
  //set all values to 0
  for	( int i = 0 ; i < NR_LED ; i ++ )	{
    np_set_pixel_rgbw(&px, i , 0, 0, 0, 0);
  }
  delay(1000);
  np_show(&px, NEOPIXEL_RMT_CHANNEL);

  //let pixels settle
  //re set 1st pixel (often glitches and stays on)
  np_set_pixel_rgbw(&px, 0 , 0, 0, 0, 0);
  delay(100);
  np_show(&px, NEOPIXEL_RMT_CHANNEL);
}
```

All of the colours used on the Arduino TFT touchscreen are defined in the `Adafruit_HX8357.h` file. In addition to the default colours provided, three more were defined for the customisation of dawn; pink, purple and orange. These colours, however, must be defined in RGB565 format - so the following were appended to the header file:
```c
#define HX8357_PINK 0xF815
#define HX8357_PURPLE 0x91F
#define HX8357_ORANGE 0xFC40
```
The dawn is set to come on exactly 4 minutes before the alarm, with proportionate brightness increments ranging from 0 (min brightness) to 255 (max brightness). This is achieved by increasing the brightness by 1/255 every 240/255 seconds (~0.94). Once a colour has been chosen from the dawn page, a method from the `main.cpp` is called to then set the dawn colour:
```c
void setDawnColour(uint16_t col) {
  if (col == HX8357_BLUE) {
    rgb = make_tuple(0,0,255); // tuple saved in RTC memory for later retrieval
  }
  ...
```
In addition to the brightness, the colour must also remain consistent by keeping the RGB ratios the same, which can be done by multiplying them by the same magnitute of brightness:
```c
void fadePixels() {
  pixBr += 0.003921569f; // 1/255
  for	( int j = 0 ; j < NR_LED ; j ++ )	{ // retrieve RGB values from saved tuple 
    np_set_pixel_rgbw(&px, j , get<0>(rgb)*pixBr, get<1>(rgb)*pixBr, get<2>(rgb)*pixBr, pixBr*255);
  }
  np_show(&px, NEOPIXEL_RMT_CHANNEL); // display on LEDs
}
```
The dawn is only active during the 4 minutes prior to the alarm, which means that any snooze executed during that time will temporarily stop the LEDs (as snoozing will add 5 minutes to the alarm time).

##### Testing 
In addition to manually observing what colours the LEDs were displaying, outputting the incremental RGB values to console during the dawn period ensured that the correct brightness and ratios were being maintained. Example **purple** dawn colour - note that brightness is rounded to 2 decimal places but the whole float (0.003921569) is used to multiply the colour values:

    Brightness: 0.00
    1, 0, 1
    Brightness: 0.01
    1, 0, 1
    Brightness: 0.01
    2, 0, 2
    Brightness: 0.02
    2, 0, 2
    Brightness: 0.02
    3, 0, 3
    Brightness: 0.02
    3, 0, 3
    Brightness: 0.03
    4, 0, 4
    Brightness: 0.03
    4, 0, 4
    Brightness: 0.04
    5, 0, 5
    Brightness: 0.04
    5, 0, 5
    Brightness: 0.04
    6, 0, 6
    Brightness: 0.05
    6, 0, 6
    Brightness: 0.05
    7, 0, 7
    ...
    
<br>
<div align = "center">
  <img src="images/dawnGif.gif" width="500"/>
</div>
<br>

### 5) Power Management
----
With any IOT device, efficient power management is a critical characteristic to manage. While it is true that many projects such as the air quality sensor need to be efficient enough to survive weeks without a charge, the context of an alarm clock means that this is not a requirement with our project. Despite this, we decided to make the device as efficient as possible so that it doesnt have to remain on charge. 

##### Deep sleeps
The best way of improving efficiency is putting the device into a deep sleep whenever possible. This is because if left in active mode the ESP requires 160-240mA current to operate - with a deep sleep this can be reduced to as little as 10uA. We decided that our alarm clock only needed to be active when the user requested it to be on, or for the 4 minutes between the dawn simulator starting and the actual alarm (plus around 20 seconds so that it can start the fade in). Because of this we implemented a feature to put the the device to sleep after 30 seconds of inactivity (i.e 30 seconds since the last registered touch), or when the used pressed button 2. In either of these cases the device can then be woken up with button 3. 

If an alarm is set however, the ESP will automatically wake itself up 20 seconds before the dawn simulator is about to begin. The ESP will then remain active up until the alarm goes off at which point it depends on user interaction;
1) If the alarm is cancelled then the device will sleep until manually woken up
2) If the alarm is snoozed (5 minutes) then the ESP will sleep for 40 seconds then wake itself up and begin the dawn again.

To improve efficiency further the ESP is disconnected from its WiFi connection and access point after the internal clock is synronized with the NTP server.

##### Peripherals
 Below is a list of the components that draw the most power and these should therefore be the focus in our effort to limit consumption. With this in mind, when the device goes into a deep sleep we limit the power they can all individually draw (see specifics below).

###### Sparkfun Open PIR module  
*Reference;  https://github.com/sparkfun/OpenPIR*

    Voltage supply range: 3VDC to 5.75VDC
    Standby average current: 80µA
    Motion-detected average current: 3mA (LED enabled) 
    
The OpenPIR consumes relatively little power. When the sensor isn't triggered --- and the activity LED is not illuminated --- the OpenPIR will consume about 80µA. When active, the onboard LED's current draw dwarfs the PIR's operating current, consuming about 3mA. This LED can however be disabled. 

Due to the insignificant amount of power drawn while active, no limitations are put in place with this component.

Maximum usage a day(1 alarmp/day): Negligible but we'll say 3mA for 1 minute

###### Neopixel featherwing - 
*Reference: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-neopixel-uberguide.pdf?timestamp=1580149439*

    USB connected: 5v
    LiPo battery: 3.7-4.2v.
    Max Current p/pixel: 60 mA at max brightness white (Red + Green + Blue)
    Avg. Current p/pixel: 20mA p/pixel -> "Rule of thumb"

From this an estimate can be made for battery life while all pixels are on;
32(pixels) * 20mA / 1000 = 0.64 Amps minimum.

The LEDS are clearly a significant draw of power. A limit could be put onto the max brightness exhibited from the individual bulbs - setting it from 255 to 127 would roughly half consumption). Yet, this save in consumption must be balanced with reducing the effectiveness of the device itself (as a wake up mechanism). For this reason we have decided not to limit its potential. It should also be noted that in our implementation max brightness is only sustained for the final second of the dawn count in. Indeed only 2 mins of the 4 min countin push past this 127 brightness level. 

Maximum usage a day(1 alarm p/day): 4 minutes of 640mA

###### LCD screen
*Reference: https://github.com/adafruit/Adafruit_HX8357_Library*

    Backlight:	18-22V,
    Current drawn: 20mA

To limit consumption, the screens backlight is turned off whenever the device is put into a deep sleep mode. Using  *IOExpander::digitalWrite(IOExpander::BACKLIGHT, LOW)*

Maximum usage a day(1 alarm p/day): 20mA for 4 minutes + 1 minute (reset alarm) = 20mA for 5 mins


###### ESP32 chip
As stated previously the chip itself consumes alot of power when in active mode;

    Current: 160-240mA

Maximum usage a day(1 alarm p/day): 5 minutes of 240mA



###### Overall rough estimate for battery life  
In an attempt to make the calculations easier we'll say all 4 components listed above use their power for 5 minutes a day. 

    240+20+640+3 = 903mA    for 5 minutes p/day. 
    With a 1200mAH battery this device should therefore last - 1200/903 * 60/5 = 15.95 days

    
    
##### Shipping mode
If the power switch is used and the ESP is not connected by USB, then the device is put into shipping mode. This tells the BM to stop supplying power and therefore means 0mA are used (the most power efficient option). This does however mean that when the device is switched back on, all the memory has been wiped and therefore any desired alarms must be reset.


### 6) Updating UI Elements 
------
The user interface implementation as a whole proved to be a lot more technical than first anticipated, especially for changing the apperance of elements on the same screen. Initially there was the idea of refreshing the entire screen each time something changed, however this approach exhibited a lot of constant flickering especially on pages that have a lot of elements or high frequency of changes such as the Home Display or Set Alarm. Instead, we devised a much more sophisticated method that involved placing filled-in rectangles over specfic areas just before they require updating:
```c
void HomeUIElement::drawTime() {
  if (sc != timeinfo->tm_sec) // clear sec if changed
    clearSec();
...
    sc = timeinfo->tm_sec; // update temp for checking previous/current value 
}
void HomeUIElement::clearSec() {
  m_tft->fillRect(  360,   90,  100,  75, BLACK);
}
```
<br>
<div align = "center">
  <img src="images/update_sketch.png" width="1000"/>
</div>
<br>

The colour of the rectangles contrast with the background in this case for illustration purposes - they would normally be the same colour to produce a seamless overlap. Each element has its own *clear* method that writes over its previous state:

<br>
<div align = "center">
  <img src="images/element_sketch.png" width="500"/>
</div>
<br>

1. **clearGreeting()** – Updates 4 times a day
2. **clearSec()** – Updates ~1s
3. **clearMin()** – Updates ~1m
4. **clearHour()** – Updates ~1h
5. **flashDots()** – Updates ~1s
6. **clearAlarm()** – Updates ~1m or ~1s depending on time to alarm
7. **clearDate()** – Updates ~24h

Another issue we discovered at this point was being able to `handletouch()` on the Home Display. This was not a challenge for other pages because they didn't need to display elements such as current time or date that update continuously, while still being able to register touches in the same dedicated process. To overcome this we needed to essentially run concurrent processes that dealt with the UI elements and touch separately; so the `draw()` method was included in the `runEachTurn()` method in Home Display. To make things even more responsive, touch in general is handled outside the UI Controller and the UI itself is only drawn once every 80 loop iterations:    

```c
if (uiCont->gotTouch()) {
  uiCont->handleTouch();
  touchTimer = millis();
}
//then, after 80 iterations, draw screen.
if (loopIter % 80 == 0) {
  uiCont->run();
}

```

### 7) Orientation & Touch Management  
------
##### Orientation
One of the first things that sprung to mind when designing the user interface was changing the orientation from portrait to landscape, as a longer horzontal axis would be ideal for the type of information the Dawn Alarm Clock would be displaying. Browsing through the `Adafruit_HX8357.cpp` library we discovered 4 different `setRotation()` methods for each orientation; 0 (portrait), 1 (portrait + 90°), 2 (portrait + 180°), 3 (portrait + 270°). However, when attempting to change the orientation to either landscapes (1 or 3) the display appeared to be mirrored. Further digging into the library we discovered `#define MADCTL_MV  0x20` which is commented as *Reverse mode*. This definition was also included in the rotation that we wanted:
```c
void Adafruit_HX8357::setRotation(uint8_t m) {
  rotation = m & 3; // can't be higher than 3
  switch(rotation) {
    case(0):
     ...
    case(1):
      m       = MADCTL_MV | MADCTL_MY | MADCTL_RGB;
      _width  = HX8357_TFTHEIGHT;
      _height = HX8357_TFTWIDTH;
      break;
     ...
  }
  sendCommand(HX8357_MADCTL, &m, 1);
}
``` 
Clearly, this had something to do with the output we were experiencing. We then found a [TFT HX8357](https://github.com/Bodmer/TFT_HX8357/blob/master/TFT_HX8357.cpp) library that had similar `setRotation()` methods to the ones in the unPhone library: 
```c
void TFT_HX8357::setRotation(uint8_t m)
{
  writecommand(HX8357_MADCTL);
  rotation = m % 8;
  switch (rotation) {
   case 0: // Portrait
     writedata(MADCTL_BGR | MADCTL_MX);
     _width  = HX8357_TFTWIDTH;
     _height = HX8357_TFTHEIGHT;
     break;
   case 1: // Landscape (Portrait + 90)
     writedata(MADCTL_BGR | MADCTL_MV);
     _width  = HX8357_TFTHEIGHT;
     _height = HX8357_TFTWIDTH;
     break;
   case 2: // Inverter portrait
     writedata( MADCTL_BGR | MADCTL_MY);
     _width  = HX8357_TFTWIDTH;
     _height = HX8357_TFTHEIGHT;
    break;
   case 3: // Inverted landscape
     writedata(MADCTL_BGR | MADCTL_MV | MADCTL_MX | MADCTL_MY);
     _width  = HX8357_TFTHEIGHT;
     _height = HX8357_TFTWIDTH;
     break;
    ...
  }
}
 ```
We then adopted `case 1: // Landscape (Portrait + 90)` which became our new `setRotation(1)` in the `Adafruit_HX8357.cpp` library which produced the desired unmirrored landscape orientation:
```c
void Adafruit_HX8357::setRotation(uint8_t m) {
  rotation = m & 3; // can't be higher than 3
  switch(rotation) {
    case 0:
    ...
    case 1:
      m       = MADCTL_BGR | MADCTL_MV;
      _width  = HX8357_TFTHEIGHT;
      _height = HX8357_TFTWIDTH;
      break;
    ...
  }
  sendCommand(HX8357_MADCTL, &m, 1);
}
```
*Reference: https://github.com/Bodmer/TFT_HX8357/blob/master/TFT_HX8357.cpp*

##### Mapping
Evidently, changing the orientaion did not come with a free lunch - the TFT screen now had to be re-mapped to correctly register points on the touchscreen. The previous configuration meant that the top right of the screen was the origin and had the x-axis running down the right side of the screen and the y-axis along the top:

<br>
<div align = "center">
  <img src="images/rotation_sketch.png" width="500"/>
</div>
<br>

The correct configuration after rotating +90° to landscape orientation would need to be mapped like the following:

<br>
<div align = "center">
  <img src="images/rotation_sketch2.png" width="500"/>
</div>
<br>

The `map()` syntax is as follows: **map(value, fromLow, fromHigh, toLow, toHigh)**. It re-maps a number from one range to another; that is, a value **fromLow** would get mapped **toLow** and a value from **fromHigh** to **toHigh** etc. The parameters are:
* **value** – the number to map
* **fromLow** – the lower bound of the value's current range
* **fromHigh** – the upper bound of the value's current range 
* **toLow** – the lower bound of the value's target range 
* **toHigh** – the upper bound of the value's target range 

So, we need to map the raw touch coordinates (**fromLow** | **fromHigh**) to the new screen coordinates (**toLow** | **toHigh**). Additionally, a **temp p.x** value will be needed in the mapping of **p.y** so that **p.y** updates correctly, otherwise only the first touch will be registered correctly and any subsequent touches will assume the initial **x** value:

```c
// raw touch calibration data 
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750
```

```c
void UIController::handleTouch() {
  // adjust mapping for landscape orientation
  // unPhone width & height --> 320 X 480
  int16_t nTmpX = p.x; // temp p.x so that p.y updates
  p.x = map(p.y, unPhone::TS_MINY, unPhone::TS_MAXY, 0, unPhone::tftp->width());
  p.y = map(nTmpX, unPhone::TS_MINX, unPhone::TS_MAXX, 0, unPhone::tftp->height());
  if(m_element->handleTouch(p.x, p.y)) {
    if(++modeChangeRequests >= MODE_CHANGE_TOUCHES) {
      changeMode();
      modeChangeRequests = 0;
    }
  }
}
```
This will position the origin at the top left of the screen and have the x-axis running along the top and y-axis running down the side. 

##### Getting Touch Point
Something that became very apparent when interacting with elements on the screen was the delayed response between registering touches. For instance, touching anywhere on the Home Display would bring up the Menu, but then selecting a menu item would not normally direct you to the desired page because it would register the previous point that was touched on the Home Display. In other words, it was executing touches in a queue that did not correspond correctly to the elements on the current page. This erroneous behaviour was due to not reading all the data in the buffer and resetting all ints when the buffer was empty:
```c
TS_Point Adafruit_STMPE610::getPoint() {
  uint16_t x, y;
  uint8_t z;
  while (!bufferEmpty()) {
    readData(&x, &y, &z);
  }
  if (bufferEmpty())
    writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints

  return TS_Point(x, y, z);
}
```
*Reference: https://github.com/adafruit/Adafruit_STMPE610/blob/master/Adafruit_STMPE610.cpp*

Furthermore, we also noticed that there would occasionally be very peculiar readings at point x ~ 16300, y ~ -12500, z ~ 25. A simple `if (p.x > 10000) {return false;}` included in the `gotTouch()` method was enough to omit this reading. 

Before implementing the buffer fix, navigation was impossible; combining these two fixes greatly increased touch accuracy and responsiveness. 

###### Testing
A very effective debugging tool that was used to test the points being registered was included in the `handleTouch()` method that displayed a tiny green box each time the screen was touched that allowed us to observe touch behaviour with ease:

```c
unPhone::tftp->fillRect(p.x-1,p.y-1,2,2,HX835 7_GREEN); // DEBUG touch feedback
```


##### Edited unPhone Library
In order to implment the above changes to orientation and touch buffer amendments were made in the unPhone project folder, specifically the `Adafruit_HX8357.cpp` and `Adafruit_STMPE610.cpp` libraries which are not part of `MyProjectThing`. Therefore, the amended versions of these libraries have been imported in the `lib` directory and referenced in the `component.mk` file: 
```c
COM3505_2019-STUDENT_LIB=../lib 
# library sources
BIGDEMOIDF_LIBS := $(UNPHONE_SRC_DIR) \
  $(COM3505_2019-STUDENT_LIB)/Adafruit_HX8357_Library \
  $(COM3505_2019-STUDENT_LIB)/Adafruit_STMPE610 \
```

### 8) Provisioning & OTA Updates
----------

###### What is provisioning?

Devices that have limited hardware can connect to a Wi-Fi network by a process called provisioning. They can either be set up in station mode which enables them to connect to a previously saved AP (access point), or in AP mode - which is achieved in most cases by using another external device (with a browser) that allows a user to supply the network information via the newly created web server by the IoT device.

###### Implementation in the system - Using WiFi manager library

1. On initial startup (or when the time and weather need to be fetched), the ESP sets up in station mode (allows to connect itself to a Wi-Fi network) and attempts to connect to a previously saved AP.

2. If unsuccessful, it moves into Access Point mode to create its own network and starts a DNS and WebServer for other devices to connect to it.

3. Connecting to the newly created AP via any Wi-Fi enabled device with a browser will prompt the user to enter the apKey (if any) of the AP.

4. Once successfully connected to the AP, a pop-up will be displayed showing the configuration portal, or attempting to visit any domain will redirect to the configuration portal due to the Captive Portal and DNS server.

5. After selecting and entering the password for one of the scanned APs, the ESP will attempt to connect – if successful, the application is now in control with an established Wi-Fi connection. If not, there is four minutes given to connect to the AP, afterwhich the ESP will restart.
    
```c++
   joinmeManageWiFi(apSSID.c_str(), apPassword.c_str());
    //...   
    //let wifi AP settle
      if (WiFi.status() != WL_CONNECTED) {
        while(WiFi.status() != WL_CONNECTED) {
          if (!powerOn()) powerMode(); //turn off if switch off
          if (micros() >= 300000000) { //4 mins to connect to AP
            ESP.restart();
          }
        }
      }
```

##### Security 

The password to the ESPs access point is stored in a seperate file called private.h rather than in plain text in the main file where the access point is setup. Then, upon compiling it is flashed into memory.  This means that if an attacker was able to snoop on the connection between Gitlab and the ESP, although they would see the compiled .bin file, it would be difficult to detect what the password was. In other words, the implementation has security through obscurity.

One aspect of the provisioning implementation that isnt secure  is the fact that the ESPs mac address is used as the APs SSID. The vulnerability of this is that if an attacker is on the same network as you they could intercept your traffic by posing as the networks router. It would therefore be a better idea to use store the SSID as a different term. 

A further insecurity is the fact that on the config portal, the password is stored as plain text and therefore if anyone was listening in on the network they would be able to see the user inputted credentials. A solution to this, would include replacing the text area password input field with a suitable password input.

### OTA

###### Why bother with OTA?

The very nature of the Internet of Things makes OTA updating a necessary requirement to ensure firmware is kept up to date. Indeed, millions of devices make-up the IOT and updating these all individually in person is neither a cost-effective nor a practical way to install the latest firmware. This is why OTA processes are so important: it enables manufacturers who incorporate the microcontroller into their device the ability to update the devices over the internet. This very fact describes the importance of provisioning - it enables OTA updates to take place. 

In regards to why we implemented it in our own system, if updates are required such as changing timezone or daylight saving hours, the implementation of an OTA update will be able to correct the error in terms of time. More importantly perhaps, it's more practical when put in the context of bugs - a quick fix can be quickly applied to all devices with an OTA implementation.

###### The implementation in the existing system


To upload the latest version of the firmware to Gitlab, so it is ready to be fetched by the ESP, the user writes ‘make push-firmware’ in the terminal. This compiles the firmware and creates a version.txt file which holds the value of the compiled firmwares version number. It then pushes these both to Gitlab in the firmware directory.

The device will check for an update whenever a connection is established during the boot process. It begins by calling a function called joinmeOTAUpdate, which in turn calls another function called joinMeCloudGet. This creates a get request to retrieve an integer value containing the latest version number of the firmware available on Gitlab. It then cross references this with the version of the firmware currently on the ESP. If it is outdated it means the firmware needs to be upgraded. The system therefore makes a final call to joinmeCloudGet which creates another get request to retrieve the (firmwareVersion).bin file from Gitlab. If this is retrieved successfully, it writes this to the ESP and restarts it. The ESP now has the latest version of the firmware successfully installed.

###### Security

The get request to Gitlab is fairly secure. The main reason for this is because the firmware updates are retrieved from Gitlab using a secure transfer protocol known as HTTPS. See the weather API section for more on this.

### 9) Circuitry
----------

See below the wiring involved in connecting the Neopixel featherwing and the Open PIR sensor - both of these are attached to the unPhone IOExpander;

<br>
<div align = "center">
  <img src="images/expander.jpg" width="500"/>
</div>
<br>

###### Sparkfun Open PIR module   (Feather wing slot #3)


    Ground: GRD
    OUT: Pin 27
    VCC (Power input): 5V
    
###### Neopixel featherwing

    
    Souldered onto 2.54mm breakway male-female header connectors
    VCC: VBAT / VUSB ( whichever gives greater V)
    Control pin: 6
    


### Size of Firmware
------

With any IOT application it is important to restrict the size of the firmware and ensure efficiency at any stage. After running the command `make size` in the directory of the project a calculation is made to estimate the size of the firmware. See below;

    Total sizes:
    DRAM .data size:    14812 bytes
    DRAM .bss  size:    34672 bytes
    Used static DRAM:   49484 bytes ( 131252 available, 27.4% used)
    Used static IRAM:   94584 bytes (  36488 available, 72.2% used)
    Flash code:         889186 bytes
    Flash rodata:       297328 bytes
    Total image size:~1295910 bytes (.bin may be padded larger)
    
As shown there is still a fair amount of space left meaning any future improvements (see below) could be sufficiently implemented. 


### Future Improvements
------
Had we had more time for the project, there were a number of ideas and features that could have made their way into the Dawn Alarm Clock;
* **Personal greeting on Home Display page** – The idea behind this was to allow the user to input their name and have it included in the greeting, for example: ***Good afternoon, Hamish***. One of the main reasons why this was not included was due to the complexities arising from text-based user input such as designing a keyboard or rotating through letters of the alphabet (similar mechanic to setting the alarm). We even considered doing it through phone (similar to provisioning) but again this would have added unecessary difficulties. 
* **Customisable dawn time** – Perhaps the most reasonable improvement that could have been made had we had a bit more time. Currently dawn is set to come on 4 minutes before the alarm and cannot be changed. 
* **Set alarm longer than 7 days into the future** – One of the most time consuming features of the project was being able to set an alarm ***up to*** 7 days into the future, from just 24 hours into the future. Implementing this feature would not have come without its unforseen technicalities.
* **Multiple / recurring  alarms** – In addition to adapting the core functionality in many areas of code, we would also need to consider changing many UI elements to accomodate more active alarms.   

### Libraries used
------
Neopixel - Using the arduino neopixel libraries gave interesting ghosting results. Ended up using a library written for a micropython project - it included functions to accurately setup the timings which can apparently be pretty specific with these featherwings. 
https://github.com/loboris/MicroPython_ESP32_psRAM_LoBo

LCD screen (Adafruit_HX8357)
https://github.com/adafruit/Adafruit_HX8357_Library

Touchscreen (STMPE610)
https://github.com/adafruit/Adafruit_STMPE610

WiFi manager (provisioning)
https://github.com/tzapu/WiFiManager

unPhone
https://gitlab.com/hamishcunningham/unphone

HTTPClient 
https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPClient

NTPClient-3.2.0
https://github.com/arduino-libraries/NTPClient

Adafruit-GFX 
https://github.com/adafruit/Adafruit-GFX-

RTC time 
https://github.com/espressif/esp-idf/blob/master/components/soc/esp32/include/soc/rtc.h
