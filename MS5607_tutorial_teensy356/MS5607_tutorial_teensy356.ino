// Example code for running the parallax altimeter module (MS5607) using teensy 3.5/6. This example uses i2c communication.
// Written by Garrett Ailts
//-------------------------------------------------------------------------------------------------------------------------
// Pin connections
//  Teensy->MS5607 (I2C)                                    
//  Pins          Description        MS5607    Description
//  18,19         SCL,SDA            SCL,SDA  (4.7k Ohm external pull-ups)
//  GND           Ground             GND      Ground
//  3V3           Power              VCC      Supply Voltage (3.3V nominal)
//  3V3           Power              PS       Protocol Select
//  GND           Ground             CS       Chip Select                 






#include <i2c_t3.h>     // This i2c library must be used to communicate with teensy 3.5/3.6. Library inlcudes the "Wire" object
                        // so its use is very similar to the wire library
#include <Arduino.h>
#include "Salus_Baro.h" // You can find this library on the github at <insert url>
                        // 

///////////Defines///////////
#define TIMER_RATE      (1000)                  // Check the timer every 1 millisecond (1/1000)
#define BARO_RATE       (TIMER_RATE / 200)      // Process MS5607 data at 100Hz
#define C2K             273.15

Salus_Baro myBaro;
float pressure = 0;
float altitude = 0;
float temperature = 0;
unsigned long prevTime = 0;
float startAlt = 0;


void setup() {
  Serial.begin(9600);
  delay(500);
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400); // Wire.begin() must inlcude more specifications for use with teensy
                                                                              // . Refer for documentation for explanation of function arguements
  // Initialize the Barometer
  Serial.println(F("Initializing Altimeter..."));
  myBaro.begin();                                   // Initialize Barometer
  startAlt = myBaro.getAltitude();                  // Request altitude data from altimeter
  Serial.print(F("Start Alt: "));
  Serial.println(startAlt);
  Serial.println(F("Barometer Initialized.\n"));
  //Wire.setRate(I2C_RATE_400);                       // Set i2c rate to 400 kHz

}

void loop() {
 if(millis()-prevTime>=1000){                       // Run main loop once per second
   prevTime = millis();
   myBaro.baroTask();                               // Request current pressure and temperature data from altimeter
   Serial.println(myBaro.getReferencePressure());   
   pressure = myBaro.getPressure()/10;              // Get pressure in pascals
   altitude = myBaro.getAltitude(); //- startAlt;   // Get msl altitude in meters
   temperature = myBaro.getTemperature()+C2K;       // Get temperature in Kelvin
   Serial.println("Pressure: " + String(pressure) + " kpa");     // Print values to serial monitor
   Serial.println("Altitude: " + String(altitude) + " m");
   Serial.println("Temperature: " + String(temperature) + " K");
   
 }

}
