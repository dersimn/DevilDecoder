#include "IRCodes.h"
#include "default_config.h"
#include "handy_functions.h"

#include <ESP8266WiFi.h>

#include <GenericLog.h>         // https://github.com/dersimn/ArduinoUnifiedLog
#include <NamedLog.h>
#include <LogHandler.h>
#include <LogSerialModule.h>
#include <LogMqttModule.h>

#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>
#include <ThreadRunOnce.h>      // https://github.com/dersimn/ArduinoThreadRunOnce

#include <PubSubClient.h>       // https://github.com/knolleary/pubsubclient
#include <PubSubClientTools.h>  // https://github.com/dersimn/ArduinoPubSubClientTools

#include <ArduinoJson.h>

#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include <OneWire.h>
#include <DallasTemperature.h>  // https://github.com/milesburton/Arduino-Temperature-Control-Library

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

LogHandler logHandler;
LogSerialModule serialModule(115200);

GenericLog Log    (logHandler);
NamedLog   LogWiFi(logHandler, "WiFi");
NamedLog   LogMqtt(logHandler, "MQTT");
NamedLog   LogDallas(logHandler, "Dallas");
NamedLog   LogVolume(logHandler, "Volume");
NamedLog   LogIRrecv(logHandler, "IRrecv");

ThreadController threadControl = ThreadController();

WiFiClient espClient;
PubSubClient mqttClient(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(mqttClient);

IRsend irsend(IR_OUT, false, false); // Do not modulate the signal
IRrecv irrecv(IR_IN);

const String ESP_ID = upperCaseStr(String(ESP.getChipId(), HEX));
const String BOARD_ID = String("DevilDecoderRemote_")+ESP_ID;
char   BOARD_ID_CHAR[50];
String s = "";

bool power = false;
channels_t current_channel = DEFAULT_CHN; 

void setup() {
  BOARD_ID.toCharArray(BOARD_ID_CHAR, 50);
  
  Serial.begin(115200);
  logHandler.addModule(&serialModule);
  Log.info("Initializing 'DevilDecoderRemote'");
  Log.info( String("ESP ID: ") + ESP_ID );

  // Init Submodules
  irsend.begin();
  irrecv.enableIRIn();
  pinMode(PWR_LED_IN, INPUT);

  setup_WiFi();
  setup_MQTT();
  setup_ArduinoOTA();

  setup_IRrecv();
  setup_VolumeHandler();

  setup_Maintanance();
  setup_Sensor_Dallas();

  Log.info("Setup done");
}

void loop() {
  static bool enteredLoop = false;
  if ( !enteredLoop ) {
    enteredLoop = true;
    Log.info("Entering loop()");

    // After everything run this once
    initDefault();
  }
  
  loop_MQTT();
  loop_IRrecv();
  loop_ArduinoOTA();

  threadControl.run();
}
