ThreadRunOnce initDefault_PowerOnThread = ThreadRunOnce();

void initDefault() {
  if (DEFAULT_PWR && !digitalRead(PWR_LED_IN)) {
    irsend.sendNEC(IR_TEUFEL_ONOFF, 32);
    
    initDefault_PowerOnThread.onRun(initDefault_PowerOnFunction);
    threadControl.add(&initDefault_PowerOnThread);

    // Takes 8sec to boot up
    initDefault_PowerOnThread.setRunOnce(8500);
  } else if (DEFAULT_PWR && digitalRead(PWR_LED_IN)) {
    initDefault_PowerOnFunction();
  }
}

void initDefault_PowerOnFunction() {
  irWrite_InputChannel(DEFAULT_CHN);

  delay(50);

  // Reads current volume on boot
  irsend.sendNEC(IR_TEUFEL_VOL_DOWN, 32, 1); // Activates Volume display
  irsend.sendNEC(IR_TEUFEL_VOL_DOWN, 32, 1); // -1
  irsend.sendNEC(IR_TEUFEL_VOL_UP,   32, 1); // +1
}

