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
  if (DEFAULT_CHN == _51)   irsend.sendNEC(IR_TEUFEL_CHANNEL_51,    32);
  if (DEFAULT_CHN == OPT1)  irsend.sendNEC(IR_TEUFEL_CHANNEL_OPT1,  32);
  if (DEFAULT_CHN == OPT2)  irsend.sendNEC(IR_TEUFEL_CHANNEL_OPT2,  32);
  if (DEFAULT_CHN == COAX1) irsend.sendNEC(IR_TEUFEL_CHANNEL_COAX1, 32);
  if (DEFAULT_CHN == COAX2) irsend.sendNEC(IR_TEUFEL_CHANNEL_COAX2, 32);
  if (DEFAULT_CHN == TV)    irsend.sendNEC(IR_TEUFEL_CHANNEL_TV,    32);
  if (DEFAULT_CHN == CD)    irsend.sendNEC(IR_TEUFEL_CHANNEL_CD,    32);
  if (DEFAULT_CHN == AUX)   irsend.sendNEC(IR_TEUFEL_CHANNEL_AUX,   32);
}

