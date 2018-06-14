ThreadRunOnce publishThread = ThreadRunOnce();

void setup_VolumeHandler() {
  publishThread.onRun(publishHifi);
  threadControl.add(&publishThread);

  // Post & Englight after initializing
  publishHifi();

  mqtt.subscribe(s+MQTT_PREFIX+"/set/"+BOARD_ID+"/hifi", hifi_subscribe);
  mqtt.subscribe(s+MQTT_PREFIX+"/set/"+BOARD_ID+"/hifi/volume", volume_subscribe);
}

void hifi_subscribe(String topic, String message) {
  DynamicJsonBuffer jsonBuffer;
  JsonVariant root = jsonBuffer.parse(message);

  if ( root.is<bool>() ) {
    pwr_set( root.as<bool>() );
    return;
  }
  if ( root.is<JsonObject>() ) {
    JsonObject& rootObject = root.as<JsonObject>();
    if ( rootObject.containsKey("val") ) {
      pwr_set( rootObject["val"].as<bool>() );
    }
    if ( rootObject.containsKey("channel") ) {
      String tmp = rootObject["channel"].as<String>();
      if (tmp == TEUFEL_CHANNELS_STRING[_51])   channel_set(_51);
      if (tmp == TEUFEL_CHANNELS_STRING[OPT1])  channel_set(OPT1);
      if (tmp == TEUFEL_CHANNELS_STRING[OPT2])  channel_set(OPT2);
      if (tmp == TEUFEL_CHANNELS_STRING[COAX1]) channel_set(COAX1);
      if (tmp == TEUFEL_CHANNELS_STRING[COAX2]) channel_set(COAX2);
      if (tmp == TEUFEL_CHANNELS_STRING[TV])    channel_set(TV);
      if (tmp == TEUFEL_CHANNELS_STRING[CD])    channel_set(CD);
      if (tmp == TEUFEL_CHANNELS_STRING[AUX])   channel_set(AUX);
    }
  }
}
void volume_subscribe(String topic, String message) {
  DynamicJsonBuffer jsonBuffer;
  JsonVariant root = jsonBuffer.parse(message);

  if ( root.is<int>() ) {
    val_set( root.as<int>() );
    return;
  }
  if ( root.is<JsonObject>() ) {
    JsonObject& rootObject = root.as<JsonObject>();
    if ( rootObject.containsKey("val") ) {
      val_set( rootObject["val"].as<int>() );
    }
  }
}
void publishHifi() {
  String output;
  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  root["val"] = power;
  root["booting"] = booting;
  root["input"] = TEUFEL_CHANNELS_STRING[current_channel];
  root["display"] = (bool)digitalRead(DISP_LED_IN);

  root["volMute"] = realVolume_mute;
  root["volFL"] = realVolume_channel[1];
  root["volFR"] = realVolume_channel[2];
  root["volC"] = realVolume_channel[3];
  root["volSUB"] = realVolume_channel[4];
  root["volRL"] = realVolume_channel[5];
  root["volRR"] = realVolume_channel[6];

  root.printTo(output);
  mqtt.publish(s+MQTT_PREFIX+"/status/"+BOARD_ID+"/hifi", output, true);
}

void val_set(int val) {
  if (val > 0) {
    irsend.sendNEC(IR_TEUFEL_VOL_UP, 32, val +1);
  }
  if (val < 0) {
    irsend.sendNEC(IR_TEUFEL_VOL_DOWN, 32, (val*-1) +1);
  }
}
void pwr_set(bool val) {
  if (power != val) {
    irsend.sendNEC(IR_TEUFEL_ONOFF, 32);
  }
}
void channel_set(channels_t val) {
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
