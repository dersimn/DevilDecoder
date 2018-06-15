#define MQTT_SERVER     "10.1.1.50"
#define WIFI_SSID       ""
#define WIFI_PASS       ""
#define MQTT_PREFIX     "dersimn"

// I/O Config
#define PWR_LED_IN       D5
#define DISP_LED_IN      D1
#define IR_IN            D6
#define IR_OUT           D0
#define READ_I2C_SCL     D2
#define READ_I2C_SDA     D7
// Defaults
#define DEFAULT_CHN       OPT1
#define DEFAULT_PWR       true
#define DISPLAY_AUTOOFF   15000
#define REFERENCE_CHANNEL 1

// Maintenance
#define MAINTENANCE_UPTIME_INTERVAL     60
#define MAINTENANCE_INFO_INTERVAL       60

// Sensor_Dallas
#define DS_ONE_WIRE_BUS       D4
#define DS_INTERVAL           60000
#define DS_PRECISION          10
