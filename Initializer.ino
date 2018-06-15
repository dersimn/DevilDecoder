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

  // This reads current volume on boot
  irWrite_VolumeDiff(-2);
}

