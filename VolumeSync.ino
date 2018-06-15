Thread syncVolumeThread = Thread();

int8_t targetVolume = 0;

void setup_VolumeSync() {
  syncVolumeThread.onRun(syncVolumeThreadFunction);
  syncVolumeThread.setInterval(5);
  syncVolumeThread.enabled = false;
  threadControl.add(&syncVolumeThread);
}

void syncVolume(int8_t volume) {
  targetVolume = volume;

  syncVolumeThread.enabled = true;
}

void syncVolumeThreadFunction() {
  if (power && realVolume_channel_valid[REFERENCE_CHANNEL]) {
    Log.info(s+"Sync Diff "+(targetVolume - realVolume_channel[REFERENCE_CHANNEL]));
    
    if ( targetVolume == realVolume_channel[REFERENCE_CHANNEL] ) {
      syncVolumeThread.enabled = false;
      return;
    }
  
    if ( targetVolume > realVolume_channel[REFERENCE_CHANNEL] ) {
      irsend.sendNEC(IR_TEUFEL_VOL_UP, 32, 1);
    } else if ( targetVolume < realVolume_channel[REFERENCE_CHANNEL] ) {
      irsend.sendNEC(IR_TEUFEL_VOL_DOWN, 32, 1);
    }
  } else {
    syncVolumeThread.enabled = false;
  }
}

