/*
  Matrix Mini R4 BLE Example
 * Description: Demonstrates how to using BLE with Nordic UART Service to control LED and send messeage to user device.

 * Author: Anthony
 * Modified 20 Dec 2025

  www.matrixrobotics.com
*/
#include <ArduinoBLE.h>
#include "MatrixMiniR4.h"

//Nordic UART Service
BLEService uartService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
BLEStringCharacteristic rxChar("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite, 128);
BLEStringCharacteristic txChar("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLERead | BLENotify, 128);

unsigned long timer_1 = 0;

String ble_rcvData = "";
void onBLEWritten(BLEDevice central, BLECharacteristic characteristic) {
  ble_rcvData = rxChar.value();

  // Send back to user (ECHO)
  txChar.writeValue(String(String("RCV:  ") + String(ble_rcvData)));
  LED_Ctrl(ble_rcvData.toFloat());
}

void LED_Ctrl(float Num) {
  if(Num == 1)
  {
    MiniR4.LED.setColor(2, 0, 0, 255);
  }
  if(Num.toFloat() == 0)
  {
    MiniR4.LED.setColor(2, 0, 0, 0);
  }
}

void setup()
{
  MiniR4.begin();
  MiniR4.PWR.setBattCell(2);
  Serial.begin(9600);
  BLE.begin();
  BLE.setLocalName("MATRIX-R4-BLE");
  BLE.setAdvertisedService(uartService);
  uartService.addCharacteristic(rxChar);
  uartService.addCharacteristic(txChar);
  BLE.addService(uartService);
  rxChar.setEventHandler(BLEWritten, onBLEWritten);
  BLE.advertise();
}

void loop()
{
  BLE.poll();
  if(!BLE.connected())
  {
    MiniR4.LED.setColor(1, 255, 0, 0);
  }
  else
  {
    MiniR4.LED.setColor(1, 0, 255, 0);
  }
  if((millis() - timer_1) > 3000)
  {
    timer_1 = millis();
    txChar.writeValue(String(String("hi") + String((random(1, 10))) + String("\n")));
  }

}