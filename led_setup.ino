/*
  LED
  
  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control an LED.
  
  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
    
  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.
  
  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

#define PIN_NEO_PIXEL_1     2   // Arduino pin that connects to NeoPixel
#define NUM_PIXELS_1        14  // The number of LEDs (pixels) on NeoPixel

#define PIN_NEO_PIXEL_2     3   // Arduino pin that connects to NeoPixel
#define NUM_PIXELS_2        26  // The number of LEDs (pixels) on NeoPixel

#define PIN_NEO_PIXEL_3     4   // Arduino pin that connects to NeoPixel
#define NUM_PIXELS_3        28  // The number of LEDs (pixels) on NeoPixel

Adafruit_NeoPixel pixel_1(NUM_PIXELS_1, PIN_NEO_PIXEL_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel_2(NUM_PIXELS_2, PIN_NEO_PIXEL_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel_3(NUM_PIXELS_3, PIN_NEO_PIXEL_3, NEO_GRB + NEO_KHZ800);

void setup() {
  //Serial.begin(9600);
  //while (!Serial);
  delay(1000);
  
  // begin initialization
  if (!BLE.begin()) {
    //Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  //Serial.println("BLE LED Peripheral");


  pixel_1.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixel_2.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixel_3.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixel_1.clear();
  pixel_1.fill(pixel_1.Color(255, 0, 0));
  pixel_1.show();
  delay(1000);
}

void loop() {
  pixel_1.clear();  pixel_2.clear();  pixel_3.clear();
  pixel_1.show();   pixel_2.show();   pixel_3.show();  
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    //Serial.print("Connected to central: ");
    // print the central's MAC address:
    //Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   // any value other than 0
          int temp_val = switchCharacteristic.value();
          if ( temp_val <= -30) {  // any value other than 0
            pixel_1.fill(pixel_1.Color(0, 255, 0),0,NUM_PIXELS_1);
            pixel_1.show();
            delay(2000);
            pixel_1.clear();
            pixel_1.show();
          } else if ( -29 <= temp_val && temp_val <= 29)      {  
            pixel_2.fill(pixel_2.Color(255, 0, 0),0,NUM_PIXELS_2);
            pixel_2.show();
            delay(2000);
            pixel_2.clear();
            pixel_2.show();
          }
          else if ( 30 <= temp_val)      {                             // a 0 value
            pixel_3.fill(pixel_3.Color(255, 0, 0),0,NUM_PIXELS_3);
            pixel_3.show();
            delay(2000);  
            pixel_3.clear();
            pixel_3.show();
          }
        }
        else {
              pixel_1.clear();  pixel_2.clear();  pixel_3.clear();
              pixel_1.show();   pixel_2.show();   pixel_3.show();  
        }
      }
    }

    // when the central disconnects, print it out:
    //Serial.print(F("Disconnected from central: "));
    //Serial.println(central.address());
  }
}
