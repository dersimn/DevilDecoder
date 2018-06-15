#define READ_I2C_BUF_SIZE1   5
#define READ_I2C_BUF_SIZE2   20
#define READ_I2C_TIMEOUT     120

ThreadRunOnce volumeReaderThread = ThreadRunOnce();

uint32_t read_i2c_last_micros = 0;
uint8_t  read_i2c_buffer[READ_I2C_BUF_SIZE1][READ_I2C_BUF_SIZE2];
int      read_i2c_cmd_pointer = -1;

void setup_volumeReader() {  
  volumeReaderThread.onRun(volumeReaderThreadFunction);
  threadControl.add(&volumeReaderThread);

  attachInterrupt(READ_I2C_SCL, i2c_read, RISING);
}

void i2c_read() {
  uint32_t this_micros = micros();
  static int pos = 1;

  // Check timeout
  if (this_micros - read_i2c_last_micros > READ_I2C_TIMEOUT) {
    // Interpret as new command
    read_i2c_cmd_pointer++;
    pos = 1;
  }

  // Read
  if (pos%9) {
    bitWrite(read_i2c_buffer[read_i2c_cmd_pointer][pos/9], 7-(pos%9-1), digitalRead(READ_I2C_SDA));
  }
  pos++;

  read_i2c_last_micros = this_micros;
  volumeReaderThread.setRunOnce(2);
}

void i2c_clear_cmd(uint8_t* cmd) {
  for (int i = 0; i < READ_I2C_BUF_SIZE2; i++) {
    cmd[i] = 0;
  }
}

void volumeReaderThreadFunction(){
  for (int i = 0; i < READ_I2C_BUF_SIZE1; i++) {
    if (read_i2c_buffer[i][0] == 0b10000000) { // Check address of PT2258

      uint8_t chan_exists[] = {0,0,0,0,0,0,0};
      int8_t chan[] = {0,0,0,0,0,0,0};
      bool mute = false;
      bool mute_exists = false;
      
      for (int j = 1; j < READ_I2C_BUF_SIZE2; j++) {
        uint8_t tmp = read_i2c_buffer[i][j];
        if (!tmp) continue;

        // Cmd table on PT2258 manual p.10
        switch ((tmp & 0b11110000) >> 4) {
          case 0b1110: chan[0] -= (tmp & 0b1111);      chan_exists[0]++; break;
          case 0b1101: chan[0] -= (tmp & 0b0111) * 10; chan_exists[0]++; break;
          case 0b0001: chan[3] -= (tmp & 0b1111);      chan_exists[3]++; break;
          case 0b0000: chan[3] -= (tmp & 0b0111) * 10; chan_exists[3]++; break;
          case 0b0011: chan[4] -= (tmp & 0b1111);      chan_exists[4]++; break;
          case 0b0010: chan[4] -= (tmp & 0b0111) * 10; chan_exists[4]++; break;
          case 0b0101: chan[2] -= (tmp & 0b1111);      chan_exists[2]++; break;
          case 0b0100: chan[2] -= (tmp & 0b0111) * 10; chan_exists[2]++; break;
          case 0b0111: chan[5] -= (tmp & 0b1111);      chan_exists[5]++; break;
          case 0b0110: chan[5] -= (tmp & 0b0111) * 10; chan_exists[5]++; break;
          case 0b1001: chan[1] -= (tmp & 0b1111);      chan_exists[1]++; break;
          case 0b1000: chan[1] -= (tmp & 0b0111) * 10; chan_exists[1]++; break;
          case 0b1011: chan[6] -= (tmp & 0b1111);      chan_exists[6]++; break;
          case 0b1010: chan[6] -= (tmp & 0b0111) * 10; chan_exists[6]++; break;
          case 0b1111: mute = (tmp & 0b1);             mute_exists = true; break;
        }
      }

      // Write changes to global variable
      for (int j = 0; j <= 6; j++) {
        if (chan_exists[j] == 2) {
          realVolume_channel[j] = chan[j] +10;
          realVolume_channel_valid[j] = true;
        }
      }
      if (mute_exists) {
        realVolume_mute = mute;
        realVolume_mute_valid = true;
      }

      // Clear the buffer that was covered by this for loop
      i2c_clear_cmd(read_i2c_buffer[i]);
    }   
  }
  
  // Reset Write Pointer
  read_i2c_cmd_pointer = -1;

  /*
  Serial.print(realVolume_mute, BIN);
  for (int k = 0; k < 7; k++) {
    Serial.print(realVolume_channel[k], DEC); Serial.print(" ");
  }
  Serial.println();
  */
  
  publishVolume();
}
