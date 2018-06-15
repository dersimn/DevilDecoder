void setup_MessageHandler() {
  mqtt.subscribe(s+MQTT_PREFIX+"/set/"+BOARD_ID+"/hifi", hifi_subscribe);
  mqtt.subscribe(s+MQTT_PREFIX+"/set/"+BOARD_ID+"/hifi/volume", volume_subscribe);
}

void hifi_subscribe(String topic, String message) {
  DynamicJsonBuffer jsonBuffer;
  JsonVariant root = jsonBuffer.parse(message);

  if ( root.is<bool>() ) {
    irWrite_Power( root.as<bool>() );
    return;
  }
  if ( root.is<JsonObject>() ) {
    JsonObject& rootObject = root.as<JsonObject>();
    if ( rootObject.containsKey("val") ) {
      irWrite_Power( rootObject["val"].as<bool>() );
    }
    if ( rootObject.containsKey("input") ) {
      String tmp = rootObject["input"].as<String>();
      if (tmp == TEUFEL_CHANNELS_STRING[_51])   irWrite_InputChannel(_51);
      if (tmp == TEUFEL_CHANNELS_STRING[OPT1])  irWrite_InputChannel(OPT1);
      if (tmp == TEUFEL_CHANNELS_STRING[OPT2])  irWrite_InputChannel(OPT2);
      if (tmp == TEUFEL_CHANNELS_STRING[COAX1]) irWrite_InputChannel(COAX1);
      if (tmp == TEUFEL_CHANNELS_STRING[COAX2]) irWrite_InputChannel(COAX2);
      if (tmp == TEUFEL_CHANNELS_STRING[TV])    irWrite_InputChannel(TV);
      if (tmp == TEUFEL_CHANNELS_STRING[CD])    irWrite_InputChannel(CD);
      if (tmp == TEUFEL_CHANNELS_STRING[AUX])   irWrite_InputChannel(AUX);
    }
    if ( rootObject.containsKey("display") ) {
      irWrite_Display( rootObject["display"].as<bool>() );
    }
    if ( rootObject.containsKey("volume") ) {
      if (power) {
        int8_t volume_int = rootObject["volume"].as<int>();
        if ( inRange( volume_int, -53, 10) ) {
          syncVolume(volume_int);
        }
      }
    }
  }
}
void volume_subscribe(String topic, String message) {
  DynamicJsonBuffer jsonBuffer;
  JsonVariant root = jsonBuffer.parse(message);

  if ( root.is<int>() ) {
    irWrite_VolumeDiff( root.as<int>() );
    return;
  }
  if ( root.is<JsonObject>() ) {
    JsonObject& rootObject = root.as<JsonObject>();
    if ( rootObject.containsKey("val") ) {
      irWrite_VolumeDiff( rootObject["val"].as<int>() );
    }
  }
}

void publishPowerState() {
  String output;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["val"] = power;
  root["booting"] = booting;

  root.printTo(output);
  mqtt.publish(s+MQTT_PREFIX+"/status/"+BOARD_ID+"/hifi/power", output, true);
}
void publishDisplayState() {
  String output;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["val"] = (bool)digitalRead(DISP_LED_IN);

  root.printTo(output);
  mqtt.publish(s+MQTT_PREFIX+"/status/"+BOARD_ID+"/hifi/display", output, true);
}
void publishInputChannel() {
  if (currentInputChannel_valid) {
    String output;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
  
    root["val"] = TEUFEL_CHANNELS_STRING[currentInputChannel];
  
    root.printTo(output);
    mqtt.publish(s+MQTT_PREFIX+"/status/"+BOARD_ID+"/hifi/input", output, true);
  }
}
void publishVolume() {
  if (realVolume_channel_valid[REFERENCE_CHANNEL]) {
    String output;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    
    root["val"] = realVolume_channel[REFERENCE_CHANNEL];
    
    if (realVolume_mute_valid) {
      root["muted"] = realVolume_mute;
    }
    
    JsonObject& channels = root.createNestedObject("channels");
    if (realVolume_channel_valid[1]) channels["FL"]  = realVolume_channel[1];
    if (realVolume_channel_valid[2]) channels["FR"]  = realVolume_channel[2];
    if (realVolume_channel_valid[3]) channels["C"]   = realVolume_channel[3];
    if (realVolume_channel_valid[4]) channels["SUB"] = realVolume_channel[4];
    if (realVolume_channel_valid[5]) channels["RL"]  = realVolume_channel[5];
    if (realVolume_channel_valid[6]) channels["RR"]  = realVolume_channel[6];
  
    root.printTo(output);
    mqtt.publish(s+MQTT_PREFIX+"/status/"+BOARD_ID+"/hifi/volume", output, true);
  }
}

