ThreadRunOnce powerChangeThread = ThreadRunOnce();

void setup_InputReader() {
  attachInterrupt(PWR_LED_IN, handle_pwr_change, CHANGE);
  attachInterrupt(DISP_LED_IN, handle_display_change, CHANGE);
  handle_pwr_change();

  powerChangeThread.onRun(powerChangeThreadFunction);
  threadControl.add(&powerChangeThread);
}

void handle_pwr_change() {
  bool tmp = digitalRead(PWR_LED_IN);
  LogVolume.info(s+"handle_pwr_change "+power);

  if (tmp && !power) {
    powerChangeThread.setRunOnce(8000);
    booting = true;
  } else {
    power = tmp;
  }
  publishHifi();
}

void handle_display_change() {
  //
}

void powerChangeThreadFunction() {
  power = true;
  booting = false;
  publishHifi();
}
