ThreadRunOnce initDefault_Channel_Thread = ThreadRunOnce();

void initDefault() {
  if (DEFAULT_PWR && !digitalRead(PWR_LED_IN)) {
    irsend.sendNEC(IR_TEUFEL_ONOFF, 32);
    
    initDefault_Channel_Thread.onRun(initDefault_Channel_function);
    threadControl.add(&initDefault_Channel_Thread);

    // Takes 8sec to boot up
    initDefault_Channel_Thread.setRunOnce(8000);
  } else if (DEFAULT_PWR && digitalRead(PWR_LED_IN)) {
    initDefault_Channel_function();
  }
}

void initDefault_Channel_function() {
  channel_set(DEFAULT_CHN);

  if (power) {
    if (DEFAULT_DISPLAY != digitalRead(DISP_LED_IN)) {
      irsend.sendNEC(IR_TEUFEL_DISPLAYONOFF, 32);
    }
  }
}

