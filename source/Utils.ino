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
