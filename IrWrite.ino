void irWrite_VolumeDiff(int val) {
  if (val > 0) {
    irsend.sendNEC(IR_TEUFEL_VOL_UP, 32, val +1);
  }
  if (val < 0) {
    irsend.sendNEC(IR_TEUFEL_VOL_DOWN, 32, (val*-1) +1);
  }
}
void irWrite_Power(bool pwr) {
  if (pwr != digitalRead(PWR_LED_IN)) {
    irsend.sendNEC(IR_TEUFEL_ONOFF, 32);
  }
}
void irWrite_InputChannel(channels_t val) {
  if (power) {
    if (val == _51)   irsend.sendNEC(IR_TEUFEL_CHANNEL_51,    32);
    if (val == OPT1)  irsend.sendNEC(IR_TEUFEL_CHANNEL_OPT1,  32);
    if (val == OPT2)  irsend.sendNEC(IR_TEUFEL_CHANNEL_OPT2,  32);
    if (val == COAX1) irsend.sendNEC(IR_TEUFEL_CHANNEL_COAX1, 32);
    if (val == COAX2) irsend.sendNEC(IR_TEUFEL_CHANNEL_COAX2, 32);
    if (val == TV)    irsend.sendNEC(IR_TEUFEL_CHANNEL_TV,    32);
    if (val == CD)    irsend.sendNEC(IR_TEUFEL_CHANNEL_CD,    32);
    if (val == AUX)   irsend.sendNEC(IR_TEUFEL_CHANNEL_AUX,   32); 
  }
}
void irWrite_Display(bool poweron) {
  if (poweron != digitalRead(DISP_LED_IN)) {
    irsend.sendNEC(IR_TEUFEL_DISPLAYONOFF, 32);
  }
}

