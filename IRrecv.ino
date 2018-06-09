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
    
    irrecv.resume();  // Receive the next value
  }
}

