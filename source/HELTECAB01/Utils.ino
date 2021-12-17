// Deep Sleep when everythings is right
void goToDeepSleep()
{
  if (ENABLE_SERIAL) {
    Serial.println("Going to sleep...");
  }
  // Prepare settings before going to sleep
  attachInterrupt(pinFlipDoor, onWakeUpByFlipDoor, FALLING);
  attachInterrupt(pinDoor, onWakeUpByDoor, FALLING);
  pinWakeUp = 0;
  // Go to sleep! Zzzz
  lowpower = 1;
}

// Deep Sleep when something's wrong
void goToDeepSleepError()
{
  if (ENABLE_SERIAL) {
    Serial.println("Going to sleep because something's wrong...");
  }
  // Prepare settings before going to sleep
  pinWakeUp = 0;
  // Go to sleep! Zzzz
  lowpower = 1;
}

// Join the Helium Network
void joinHeliumNetwork() {
  // Initialize LaRaWAN settings
  LoRaWAN.begin(LORAWAN_CLASS, ACTIVE_REGION);

  //Enable ADR
  LoRaWAN.setAdaptiveDR(true);

  while (1) {
    if (ENABLE_SERIAL) {
      Serial.print("JOINING... ");
    }
    LoRaWAN.joinOTAA(appEui, appKey, devEui);
    if (!LoRaWAN.isJoined()) {
      if (ENABLE_SERIAL) {
        Serial.println("JOIN FAILED!");
      }
    } else {
      if (ENABLE_SERIAL) {
        Serial.println("JOINED");
      }
      break;
    }
  }
}

/* Prepares the payload of the frame */
uint8_t prepareTxFrame(char code, boolean setVoltage) {
  appData[0] = code;
  if (setVoltage) {
    batteryVoltage = getBatteryVoltage();
    if (ENABLE_SERIAL) {
      Serial.println("Voltage in mV: " + batteryVoltage);
    }
    appData[1] = highByte(batteryVoltage);
    appData[2] = lowByte(batteryVoltage);
  }
  if (!setVoltage) {
    return 1;
  } else {
    return 3;
  }
}
