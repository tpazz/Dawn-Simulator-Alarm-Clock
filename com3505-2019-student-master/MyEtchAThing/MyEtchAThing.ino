// MyEtchAThing.ino, by Gareth
// an example derived from https://learn.adafruit.com/lsm303-accelerometer-slash-compass-breakout?view=all#accelerometer-demo-code-5-24
// of using the unPhone's LSM303 MEMs motion sensor

#include <SPI.h>                // the SPI bus
#include <Adafruit_GFX.h>       // core graphics library
#include "Adafruit_HX8357.h"    // tft display local hacked version
#include <Wire.h>               // I²C comms on the Arduino
#include "IOExpander.h"         // unPhone's IOExpander (controlled via I²C)
#include <Adafruit_Sensor.h>    // base class etc. for sensor abstraction
#include <Adafruit_LSM303_U.h>  // the accelerometer sensor

// power management chip config
byte I2Cadd = 0x6b;      // I2C address of the PMU
byte BM_Watchdog = 0x05; // charge termination/timer control register
byte BM_OpCon    = 0x07; // misc operation control register
byte BM_Status   = 0x08; // system status register 
byte BM_Version  = 0x0a; // vender / part / revision status register 
void checkPowerSwitch(); // what position is the on/off switch in?
void setRegister(byte, byte);   // PMU...
byte getRegister(byte);         //    ...I2C...
void write8(byte, byte, byte);  //       ...helper...
byte read8(byte, byte);         //          ...functions
void setShipping(bool);  // tell the PMU to sleep

// the LCD
#define TFT_DC   21      // this isn't on the IO expander
Adafruit_HX8357 tft =
  Adafruit_HX8357(IOExpander::LCD_CS, TFT_DC, IOExpander::LCD_RESET);

// the accelerometer (which we're using for attitude sensing)
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

// x and y coords of the etching pen
int penx = 160; // put the pen in the ...
int peny = 240; // ... middle of the screen

// setup ////////////////////////////////////////////////////////////////////
void setup(void) {
  Serial.begin(115200);
  Serial.println("version 2, adapted to unPhone 6v2 Nov 2019");
  Serial.println("etch-a-sketch by gareth");

  /* initialise the sensor */
  Wire.setClock(100000);
  if(!accel.begin()) {
    /* there was a problem detecting the sensor ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  IOExpander::begin();
  checkPowerSwitch(); // check if the power switch is now off & if so shutdown
    
  tft.begin(HX8357D); // start the LCD driver chip
  IOExpander::digitalWrite(IOExpander::BACKLIGHT,HIGH); // backlight on

  // initialise the background on screen
  tft.fillScreen(HX8357_BLACK);
  tft.drawLine(0,0,319,0,HX8357_WHITE);
  tft.drawLine(319,0,319,479,HX8357_WHITE);
  tft.drawLine(319,479,0,479,HX8357_WHITE);
  tft.drawLine(0,479,0,0,HX8357_WHITE);
}

// loop /////////////////////////////////////////////////////////////////////
void loop(void) {
  checkPowerSwitch(); // check if the power switch is now off & if so shutdown
  
  /* get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

  /*
  TODO your code here....

  Use event.acceleration.x/y and penx/y to figure out where to draw. The
  latter are initialised to the center of the screen, so the task is to
  interpret the current values of x and y (set in getEvent above) and the 
  position of the pen (if it is already at the edge of the screen it can't
  move).
  
  For more details of how this works see
  https://learn.adafruit.com/lsm303-accelerometer-slash-compass-breakout?view=all#accelerometer-demo-code-5-24

  Stuck? No clue? An answer may be found lower down if you look carefully ;)
  */

  // draw a pixel at penx/peny
  tft.drawPixel(penx, peny, HX8357_WHITE);
  
  // display events (acceleration is measured in m/s^2) */
  /*
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
  Serial.println("m/s^2 ");
  */

  /* delay before the next sample */
  delay(20);
}

// power management chip API /////////////////////////////////////////////////
void checkPowerSwitch() {
  uint8_t inputPwrSw = IOExpander::digitalRead(IOExpander::POWER_SWITCH);

  // bit 2 of status register indicates if USB connected
  bool powerGood = bitRead(getRegister(BM_Status),2);

  if(!inputPwrSw) {  // when power switch off
    if(!powerGood) { // and usb unplugged we go into shipping mode
      Serial.println("Setting shipping mode to true");
      Serial.print("Status is: ");
      Serial.println(getRegister(BM_Status));
      delay(500); // allow serial buffer to empty
      setShipping(true);
    } else { // power switch off and usb plugged in we sleep
      Serial.println("sleeping now");
      Serial.print("Status is: ");
      Serial.println(getRegister(BM_Status));
      delay(500); // allow serial buffer to empty
      esp_sleep_enable_timer_wakeup(1000000); // sleep time is in uSec
      esp_deep_sleep_start();
    }
  }
}
void setShipping(bool value) { // put the bat man into shipping mode: sleep
  byte result;
  if(value) {
    result=getRegister(BM_Watchdog); // read current state of timing register
    bitClear(result, 5);             // clear bit 5
    bitClear(result, 4);             // and bit 4
    setRegister(BM_Watchdog,result); // disable watchdog tmr (REG05[5:4] = 00)

    result=getRegister(BM_OpCon);    // current state of operational register
    bitSet(result, 5);               // set bit 5
    setRegister(BM_OpCon,result);    // to disable BATFET (REG07[5] = 1)
  } else {
    result=getRegister(BM_Watchdog); // Read current state of timing register
    bitClear(result, 5);             // clear bit 5
    bitSet(result, 4);               // and set bit 4
    setRegister(BM_Watchdog,result); // enable watchdog tmr (REG05[5:4] = 01)

    result=getRegister(BM_OpCon);    // current state of operational register
    bitClear(result, 5);             // clear bit 5
    setRegister(BM_OpCon,result);    // to enable BATFET (REG07[5] = 0)
  }
}

// I2C helpers to drive the power management chip
void setRegister(byte reg, byte value) {
  write8(I2Cadd, reg, value);
}
byte getRegister(byte reg) {
  byte result;
  result=read8(I2Cadd, reg);
  return result;
}
void write8(byte address, byte reg, byte value) {
  Wire.beginTransmission(address);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  Wire.endTransmission();
}
byte read8(byte address, byte reg) {
  byte value;
  Wire.beginTransmission(address);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom(address, (byte)1);
  value = Wire.read();
  Wire.endTransmission();
  return value;
}

/* secret code :)
  if(event.acceleration.y > 2 & penx < 318)
    penx = penx +1;
  if(event.acceleration.y < -2 & penx > 1)
    penx = penx -1;
  if(event.acceleration.x > 2 & peny < 478)
    peny = peny +1;
  if(event.acceleration.x < -2 & peny > 1)
    peny = peny -1;
*/
