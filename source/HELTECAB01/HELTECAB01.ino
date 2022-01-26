//####################### Vincent Huynen #######################/
//################## vincent.huynen@gmail.com ##################/
//########################## JANUARY 2022 ######################/
//#################### CubeCell HELTEC AB01 ####################/
//######################## Version 1.0.0 #######################/

/*
  LoRaWan Mailbox Notifier
*/

#include "Arduino.h"
#include "LoRaWan_Easy.h"

/* OTAA settings from HELIUM Network*/
uint8_t devEui[] = {  };
uint8_t appEui[] = {  };
uint8_t appKey[] = {  };

/* ABP para*/
uint8_t nwkSKey[] = {};
uint8_t appSKey[] = {};
uint32_t devAddr =  (uint32_t)0x00;

int wake_count = 0;
int batteryVoltage, batteryLevel;

// Control properties
long overtime_open_door = 20000;

// Pin which handle the wake up
int pinFlipDoor = GPIO1;
int pinDoor = GPIO2;
int pinWakeUp = 0;
unsigned long currentMillis = 0;

uint8_t lowpower = 0;

bool ENABLE_SERIAL = false; // Enable serial debug output here if required
bool ENABLE_LORAWAN = true; // Disable LoRaWAN here in debug mode

/* LoRaWan settings */
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;
DeviceClass_t  loraWanClass = LORAWAN_CLASS;
bool overTheAirActivation = LORAWAN_NETMODE;
bool loraWanAdr = LORAWAN_ADR;
//bool keepNet = LORAWAN_NET_RESERVE;
bool isTxConfirmed = LORAWAN_UPLINKMODE;
uint8_t appPort = 1;
uint8_t confirmedNbTrials = 4;

void onWakeUpByFlipDoor()
{
  lowpower = 0;
  pinWakeUp = pinFlipDoor;
  wake_count = wake_count + 1;
  detachInterrupt(pinFlipDoor);
  detachInterrupt(pinDoor);
  // Initialize current time
  currentMillis = millis();
  if (ENABLE_SERIAL) {
    Serial.println("WakeUp by the Flip Door");
  }
}

void onWakeUpByDoor()
{
  lowpower = 0;
  pinWakeUp = pinDoor;
  wake_count = wake_count + 1;
  detachInterrupt(pinFlipDoor);
  detachInterrupt(pinDoor);
  // Initialize current time
  currentMillis = millis();
  if (ENABLE_SERIAL) {
    Serial.println("WakeUp by the Door");
  }
}

void setup() {
  if (ENABLE_SERIAL) {
    Serial.begin(115200);
  }
  // Initialize interrupt flip door's GPIO
  pinMode(GPIO1, INPUT_PULLUP);
  // Init interrupt door's GPIO
  pinMode(GPIO2, INPUT_PULLUP);
  // Initialize current time
  currentMillis = millis();
}

void loop()
{
  // Handling low power mode
  if (lowpower) {
    if (ENABLE_SERIAL) {
      Serial.println("Go to sleep! Zzzz");
    }
    lowPowerHandler();
  }

  if (!lowpower) {
    // When you turn on the controller a first time
    if (wake_count == 0) {
      if (digitalRead(pinDoor) == 1 && digitalRead(pinFlipDoor) == 1) {
        if (ENABLE_LORAWAN) {
          joinHeliumNetwork();
          LoRaWAN.send(prepareTxFrame(0x01, true), 1, false);
        }
        // Only if all doors are closed
        if (ENABLE_SERIAL) {
          Serial.println("All doors are closed");
        }
        goToDeepSleep();
      } else if (millis() > overtime_open_door) {
        if (ENABLE_SERIAL) {
          Serial.println("Doors have been opened too long when you have turned on the MCU.\nReboot the MCU to reinitialize it !");
        }
        if (ENABLE_LORAWAN) {
          joinHeliumNetwork();
          LoRaWAN.send(prepareTxFrame(0x04, false), 2, false);
        }
        goToDeepSleepError();
      }
    }

    // Ignore wake up when you fetch your mail, open the flip door first before open the principal door
    if (pinWakeUp == pinFlipDoor) {
      if (digitalRead(pinDoor) == 0) {
        while (digitalRead(pinDoor) == 0  || digitalRead(pinFlipDoor) == 0)  {
          if (millis() > currentMillis + overtime_open_door) {
            if (ENABLE_SERIAL) {
              Serial.println("Doors have been opened too long time when you have fetched your mail.\nReboot the MCU for reinitialize it !");
            }
            if (ENABLE_LORAWAN) {
              LoRaWAN.send(prepareTxFrame(0x05, false), 2, false);
            }
            goToDeepSleepError();
            break;
          }
        }
        if (!lowpower) {
          // Everythings is OK, you can sleep quietly
          if (ENABLE_SERIAL) {
            Serial.println("Everythings is OK, you can sleep quietly");
          }
          goToDeepSleep();
        }
      }
    }
    // You have got mail ! Let's go...
    if (pinWakeUp == pinDoor || pinWakeUp == pinFlipDoor) {
      if (digitalRead(pinDoor) == 1  && digitalRead(pinFlipDoor) == 1) {
        if (pinWakeUp == pinDoor) {
          if (ENABLE_SERIAL) {
            Serial.println("You have received a parcel !");
          }
          if (ENABLE_LORAWAN) {
            LoRaWAN.send(prepareTxFrame(0x02, true), 1, false);
          }
        }
        if (pinWakeUp == pinFlipDoor) {
          if (ENABLE_SERIAL) {
            Serial.println("You have received a letter !");
          }
          if (ENABLE_LORAWAN) {
            LoRaWAN.send(prepareTxFrame(0x03, true), 1, false);
          }
        }
        // Everythings is OK, you can sleep quietly
        goToDeepSleep();
      }
      if (millis() > currentMillis + overtime_open_door) {
        // Send error
        if (ENABLE_SERIAL) {
          Serial.println("Doors have been opened too long time by the postman.\nReboot the MCU for reinitialize it !");
        }
        if (ENABLE_LORAWAN) {
          LoRaWAN.send(prepareTxFrame(0x06, false), 2, false);
        }
        // Something wrong, going to sleep until reset or restart
        goToDeepSleepError();
      }
    }
  }
}
