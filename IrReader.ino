decode_results ir_results;

void loop_IrReader() {
  if (irrecv.decode(&ir_results)) {
    // Decoderstation uses NEC Codec
    if (power && ir_results.decode_type == NEC) {
      switch (ir_results.value) {
        case IR_TEUFEL_CHANNEL_51:      currentInputChannel = _51;   currentInputChannel_valid = true; break;
        case IR_TEUFEL_CHANNEL_OPT1:    currentInputChannel = OPT1;  currentInputChannel_valid = true; break;
        case IR_TEUFEL_CHANNEL_OPT2:    currentInputChannel = OPT2;  currentInputChannel_valid = true; break;
        case IR_TEUFEL_CHANNEL_COAX1:   currentInputChannel = COAX1; currentInputChannel_valid = true; break;
        case IR_TEUFEL_CHANNEL_COAX2:   currentInputChannel = COAX2; currentInputChannel_valid = true; break;
        case IR_TEUFEL_CHANNEL_TV:      currentInputChannel = TV;    currentInputChannel_valid = true; break;
        case IR_TEUFEL_CHANNEL_CD:      currentInputChannel = CD;    currentInputChannel_valid = true; break;
        case IR_TEUFEL_CHANNEL_AUX:     currentInputChannel = AUX;   currentInputChannel_valid = true; break;
      }

      // Display Auto-Off
      displayAutoOff_postpone();

      // Update MQTT
      publishInputChannel();
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
    mqtt.publish(s+MQTT_PREFIX+"/status/"+BOARD_ID+"/ir", output, false);

    // Receive the next value
    irrecv.resume();
  }
}

