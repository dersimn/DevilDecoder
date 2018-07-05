ThreadRunOnce powerChangeThread = ThreadRunOnce();
ThreadRunOnce displayAutoOffThread = ThreadRunOnce();

void setup_InputReader() {
  attachInterrupt(PWR_LED_IN, handle_pwr_change, CHANGE);
  attachInterrupt(DISP_LED_IN, handle_display_change, CHANGE);
  handle_pwr_change();

  powerChangeThread.onRun(powerChangeThreadFunction);
  displayAutoOffThread.onRun(displayAutoOffThreadFunction);
  threadControl.add(&powerChangeThread);
  threadControl.add(&displayAutoOffThread);
}

void handle_pwr_change() {
  bool tmp = digitalRead(PWR_LED_IN);

  if (tmp && !power) {
    powerChangeThread.setRunOnce(8000);
    booting = true;
  } else {
    power = tmp;
  }
  publishPowerState();
}
void handle_display_change() {
  publishDisplayState();

  if (digitalRead(DISP_LED_IN)) {
    displayAutoOff_postpone();
  }
}

void displayAutoOff_postpone() {  
  // Auto-Off Display
  if (DISPLAY_AUTOOFF) {
    displayAutoOffThread.setRunOnce(DISPLAY_AUTOOFF); 
  }
}

void powerChangeThreadFunction() {
  power = true;
  booting = false;
  publishPowerState();
}
void displayAutoOffThreadFunction() {
  irWrite_Display(false);
}

