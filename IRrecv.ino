decode_results ir_results;

void setup_IRrecv() {
  //
}

void loop_IRrecv() {
  if (irrecv.decode(&ir_results)) {
    // Decoderstation uses NEC Codec
    if (ir_results.decode_type == NEC) {
      switch (ir_results.value) {
        case IR_TEUFEL_CHANNEL_51:      current_channel = _51;   break;
        case IR_TEUFEL_CHANNEL_OPT1:    current_channel = OPT1;  break;
        case IR_TEUFEL_CHANNEL_OPT2:    current_channel = OPT2;  break;
        case IR_TEUFEL_CHANNEL_COAX1:   current_channel = COAX1; break;
        case IR_TEUFEL_CHANNEL_COAX2:   current_channel = COAX2; break;
        case IR_TEUFEL_CHANNEL_TV:      current_channel = TV;    break;
        case IR_TEUFEL_CHANNEL_CD:      current_channel = CD;    break;
        case IR_TEUFEL_CHANNEL_AUX:     current_channel = AUX;   break;
      }
      publishHifi();
    }

    // Everything else
    LogIRrecv.info(s+"Decoded "+typeToString(ir_results.decode_type, false)+" "+uint64ToString(ir_results.value, 16)+" "+ir_results.repeat);

    // Publish
    String output;
    static uint64_t val;
    static int repeat = 0;
    DynamicJsonBuffer jsonBuffer;

    if (!ir_results.repeat) {
      val = ir_results.value;
      repeat = 0;
    } else {
      repeat++;
    }
  
    JsonObject& root = jsonBuffer.createObject();
    root["val"] = uint64ToString(val, 16);
    root["codec"] = typeToString(ir_results.decode_type, false);
    root["repeat"] = repeat;
  
    root.printTo(output);
    mqtt.publish(s+MQTT_PREFIX+"/status/"+BOARD_ID+"/ir", output, true);

    // Receive the next value
    irrecv.resume();
  }
}

