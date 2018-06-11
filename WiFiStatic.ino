Thread wifiThread = Thread();

void setup_WiFi() {
  WiFi.softAPdisconnect(true);
  LogWiFi.info(s+"Connecting to Wifi SSID: "+WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  for (int i = 0; i < 10; i++) {
    if (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Log.info(String("Connected with IP: ") + WiFi.localIP().toString() );
  } else {
    Log.warn("Could NOT connect to WiFi");
  }

  wifiThread.onRun(wifiFunction);
  wifiThread.setInterval(60 * 1000);
  threadControl.add(&wifiThread);
}

void wifiFunction() {
  if (WiFi.status() != WL_CONNECTED) {
    LogWiFi.warn("Connection lost");
  }
}
