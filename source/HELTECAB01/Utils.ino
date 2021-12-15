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
      if (ENABLE_LORAWAN) {
        appData[0] = 0x06;
        LoRaWAN.send(1, &appData[0], 2, false);
      }
      if (ENABLE_SERIAL) {
        Serial.println("JOINED");
      }
      break;
    }
  }


}
