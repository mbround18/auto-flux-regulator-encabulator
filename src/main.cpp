// Include necessary libraries
#include "M5CoreInk.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <sys/_intsup.h>

#include "../lib/CustomServerCallbacks/src/CustomServerCallbacks.h"


// Define UUIDs for the BLE service and characteristic
#define SERVICE_UUID       "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Initialize the Ink display and variables
Ink_Sprite InkPageSprite(&M5.M5Ink);

bool isConnected = false;
bool hasBeenCleared = false;
BLECharacteristic *pCharacteristic;
uint16_t ledState = 0;

// Function to set the LED state and update the display
void setLedState(const int state) {
 ledState = state;
 if (ledState == 0) {
   InkPageSprite.drawString(35,50,"Led State: off");
 } else {
   InkPageSprite.drawString(35,50,"Led State: on ");
 }
 InkPageSprite.pushSprite(); //Push the sprite to the screen.
}

// Define characteristic callbacks
class CharacteristicCallback : public BLECharacteristicCallbacks {
 void onWrite(BLECharacteristic *pCharacteristic) override {
   const std::string value = pCharacteristic->getValue();
   setLedState(std::stoi(value));
 }
 void onRead(BLECharacteristic *pCharacteristic) override {
   if (ledState == 0) {
     InkPageSprite.drawString(35,50,"Led State: off");
     pCharacteristic->setValue(ledState);
   } else {
     InkPageSprite.drawString(35,50,"Led State: on ");
     pCharacteristic->setValue(ledState);
   }
 }
};

CustomServerCallbacks *pCallbacks = new CustomServerCallbacks();


// Setup function
void setup() {



   // Initialize serial communication, M5 device, and display
   Serial.begin(115200);
   M5.begin(true, false, true);
   if( !M5.M5Ink.isInit()) {
       Serial.printf("Ink Init failed");
   }
   M5.M5Ink.clear();
   delay(1000);
   if( InkPageSprite.creatSprite(0,0,200,200,true) != 0) {
       Serial.printf("Ink Sprite create failed");
   }
   InkPageSprite.drawString(35,50,"BrunOS!");
   InkPageSprite.pushSprite();

   // Initialize BLE device and server
   BLEDevice::init("BrunOS Controller");
   BLEServer *pServer = BLEDevice::createServer();
   pServer->setCallbacks(pCallbacks);

   // Create BLE service and characteristic
   BLEService *pService = pServer->createService(SERVICE_UUID);
   BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                  CHARACTERISTIC_UUID,
                                  BLECharacteristic::PROPERTY_READ |
                                  BLECharacteristic::PROPERTY_WRITE |
                                  BLECharacteristic::PROPERTY_INDICATE|
                                  BLECharacteristic::PROPERTY_NOTIFY
                                );
   pCharacteristic->setCallbacks(new CharacteristicCallback());
   uint16_t initialValue = 0;
   pCharacteristic->setValue(initialValue);
   pService->start();

   // Start advertising the BLE service
   BLEAdvertising *pAdvertising = pServer->getAdvertising();
   pAdvertising->addServiceUUID(SERVICE_UUID);
   pAdvertising->setScanResponse(true);
   pAdvertising->setMinPreferred(0x06);
   pAdvertising->setMinPreferred(0x12);
   BLEDevice::startAdvertising();
   Serial.println("Characteristic defined! Now you can read it in your phone!");
}

// Main loop
void loop() {
 if (pCallbacks->isConnected) {
   if (!hasBeenCleared) {
     M5.M5Ink.clear();
     hasBeenCleared = true;
   }
 }

 if( M5.BtnDOWN.wasPressed()){
   setLedState(1);
 } else if( M5.BtnUP.wasPressed()){
   setLedState(0);
 }
 M5.update();
}
