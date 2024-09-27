#include "Adafruit_VL53L1X.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

//LiquidCrystal LCD(10,9,7,6,5,4); //LCD connected to these arduino DC pins
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
// OLED FeatherWing buttons map to different pins depending on board:

#define IRQ_PIN 2
#define XSHUT_PIN 3

Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);

int max_volume;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println(F("Adafruit VL53L1X sensor demo"));

  Wire.begin();
  if (! vl53.begin(0x29, &Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(10);
  }
  Serial.println(F("VL53L1X sensor OK!"));

  Serial.print(F("Sensor ID: 0x"));
  Serial.println(vl53.sensorID(), HEX);

  if (! vl53.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(250); display.begin(0x3C,true);
  }
  Serial.println(F("Ranging started"));
  
  display.clearDisplay();
  display.display();
  // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
  vl53.setTimingBudget(50);
  Serial.print(F("Timing budget (ms): "));
  Serial.println(vl53.getTimingBudget());

  //LCD.begin(16,2); //turn on lcd and tell arduino how big it is
  //LCD.setCursor(0,0); //put cursor in upper left corner of lcd display
  //LCD.print("Air Volume: "); //print first row of lcd display

  /*
  vl.VL53L1X_SetDistanceThreshold(100, 300, 3, 1);
  vl.VL53L1X_SetInterruptPolarity(0);
  */
}

void loop() {
  int16_t distance;
  int volume; 
  

  if (vl53.dataReady()) {
    // new measurement for the taking!
    distance = vl53.distance();
    if (distance == -1) {
      // something went wrong!
      Serial.print(F("Couldn't get volume: "));
      Serial.println(vl53.vl_status);
      return;
    }
    volume= (distance* (4000/150));
    //volume= distance * M_PI * 8.5; //volume of cylinder in mL  
    //volume= 4000-volume;
    Serial.print(F("Air Volume: "));
    Serial.print(volume);
    Serial.println(" mL");

    // data is read out, time for another reading!
    vl53.clearInterrupt();
  }
  volume= (distance* (4000/150));
  //volume= distance * M_PI * 8.5; //volume of cylinder in mL  
  //volume= 4000-volume;
   if (volume >= max_volume) {
     max_volume = volume;
     Serial.print("Update max");
     display.display();
     delay(1000);

     // Clear the buffer.
     display.clearDisplay();
     display.display();

     display.setRotation(1);
     Serial.println("Button test");

     

     // text display tests
     display.begin(128,64);
     display.setTextSize(1);
     display.setTextColor(SH110X_WHITE);
     display.setCursor(0,0);
     display.print("Air volume");
     display.print("max Air Volume");
     display.println("");
     display.println("Sending val #0");
     display.display(); // actually display all of the above
    }

  }
   
  //LCD.setCursor(0,1); //set cursor to bottom left of lcdisplay
  //LCD.print("                "); //clear row two to avoid ghost characters
  //LCD.setCursor(0,1); //set cursor back to bottom left
  //LCD.print(max_volume); //print distance on LCD
  //LCD.print(" mL"); //set unit dont forget space to not overcrowd 
  //delay(700);   //pause to avoid jittery display
