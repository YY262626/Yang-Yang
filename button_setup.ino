/*
  LED Control
  This example scans for Bluetooth® Low Energy peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the Bluetooth® Low Energy peripheral's LED, when the button is pressed or released.
  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button with pull-up resistor connected to pin 2.
  You can use it with another board that is compatible with this library and the
  Peripherals -> LED example.
  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>


// variables for button
const int buttonPin = 2;
bool oldButtonState = LOW;

void setup() {
  //Serial.begin(9600);
  //while (!Serial);
  
  if (!IMU.begin()) {
    //Serial.println("Failed to initialize IMU!");
    while (1);
  }
//  Serial.print("Accelerometer sample rate = ");
//  Serial.print(IMU.accelerationSampleRate());
//  Serial.println(" Hz");
//  Serial.println();
//  Serial.println("Acceleration in g's");
//  Serial.println("X\tY\tZ");

  // configure the button pin as input
  pinMode(buttonPin, INPUT_PULLUP);

  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();

  //Serial.println("Bluetooth® Low Energy Central - LED control");

  // start scanning for peripherals
  BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1214");  // Hier wurde die UUID am Anfang geändert. 
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
//    Serial.print("Found ");
//    Serial.print(peripheral.address());
//    Serial.print(" '");
//    Serial.print(peripheral.localName());
//    Serial.print("' ");
//    Serial.print(peripheral.advertisedServiceUuid());
//    Serial.println();

    if (peripheral.localName() != "LED") {
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1214");
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  //Serial.println("Connecting ...");

  if (peripheral.connect()) {
    //Serial.println("Connected");
  } else {
    //Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  //Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    //Serial.println("Attributes discovered");
  } else {
    //Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic("19B10001-E8F2-537E-4F6C-D104768A1214");

  if (!ledCharacteristic) {
    //Serial.println("Peripheral does not have LED characteristic!");
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.canWrite()) {
    //Serial.println("Peripheral does not have a writable LED characteristic!");
    peripheral.disconnect();
    return;
  }
  ledCharacteristic.writeValue((int32_t)0);

  while (peripheral.connected()) {
    // while the peripheral is connected
    float x, y, z;
    IMU.readAcceleration(x, y, z);

       
    // read the button pin
    bool buttonState = digitalRead(buttonPin);

    if (buttonState == HIGH && oldButtonState == LOW) {
      // button changed
        x = x*100;
        //Serial.println(x);
        ledCharacteristic.writeValue((int32_t)x);
      }
      
      oldButtonState = buttonState;

    
  }

  //Serial.println("Peripheral disconnected");
}
