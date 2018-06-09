ThreadRunOnce publishThread = ThreadRunOnce();

void setup_VolumeHandler() {
  attachInterrupt(PWR_LED_IN, handle_pwr_change, CHANGE);
  handle_pwr_change();
  
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
  root["channel"] = TEUFEL_CHANNELS_STRING[current_channel];

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

void handle_pwr_change() {
  power = digitalRead(PWR_LED_IN);
  LogVolume.info(s+"handle_pwr_change "+power);

  publishHifi();
}
