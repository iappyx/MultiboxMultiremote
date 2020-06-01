// Begin of config

#define CONFIG_SSID "<SSID>"
#define CONFIG_PSK "<PSK>"

#define CONFIG_MQTT_IP 192, 168, 2, 5 // Only IPAdresses allowed, in format xxx, xxx, xxx, xxx
#define CONFIG_MQTT_USER "<USER>"
#define CONFIG_MQTT_PASS "<PASS>"
#define CONFIG_MQTT_CLIENTID "ITHOREMOTEV10" // Must be unique on the MQTT network

#define CONFIG_CC1101_PIN 5
#define CONFIG_NUMBEROFITHOBOXES 2 // We avoid dynamically calculating this, to speed up the loops, configure boxes and remotes in config_boxes_and_remotes.h
#define CONFIG_MAXREMOTESPERBOX 3 // We avoid dynamically calculating this, to speed up the loops, configure boxes and remotes in config_boxes_and_remotes.h
#define CONFIG_MAXCOMMANDSPERREMOTE 8 // We avoid dynamically calculating this, to speed up the loops, configure boxes and remotes in config_boxes_and_remotes.h
#define CONFIG_MAXBITSREMOTEID 3 // This is typically not changed - only used if some remotes behave differently
#define CONFIG_HOST "itho-mbmr"
#define CONFIG_HTTP_PORT 80
#define CONFIG_DHT_ENABLE 1 // 0 = disable and 1 = enable
#define CONFIG_DHT_PIN 4
#define CONFIG_DHT_TYPE DHT22
#define DHTTYPE CONFIG_DHT_TYPE

#define CONFIG_MQTTMAINTOPIC "ITHO/"
#define CONFIG_MQTTSTATUSTOPIC "ITHO/Status"
#define CONFIG_MQTTLOGTOPIC "ITHO/Log"
#define CONFIG_MQTTTEMPERATURETOPIC "ITHO/Temperature"
#define CONFIG_MQTTHUMIDITYTOPIC "ITHO/Humidity"
#define CONFIG_MQTTWILLMESSAGE "offline"
#define CONFIG_MQTTSUBTOPIC "/Fan"
#define CONFIG_MQTTCOMMANDTOPIC "/Cmd"

// End of config
// Don't forget to configure your boxes and remotes in config_boxes_and_remotes.h

// Initialize objects and initial settings
const int DHTPIN = CONFIG_DHT_PIN;
DHT dht(DHTPIN, DHTTYPE);
long dhtEnabled = CONFIG_DHT_ENABLE;
float dhtTemperature = 0;
float dhtHumidity = 0;
int maxCommandsPerRemote = CONFIG_MAXCOMMANDSPERREMOTE;
int maxBitsRemoteId = CONFIG_MAXBITSREMOTEID;

const char *host = CONFIG_HOST;
const char *ssid = CONFIG_SSID;
const char *password = CONFIG_PSK;
int8_t IRQ_pin = CONFIG_CC1101_PIN;
bool configLog = true;
bool loopLoop = true;
bool firstRun = true;
static unsigned long lastRefreshTime = 0;
static unsigned long lastRefreshTimeDHT = 0;

ESP8266WebServer httpServer(CONFIG_HTTP_PORT);
ESP8266HTTPUpdateServer httpUpdater;
WiFiClient client;
IPAddress mqttServer(CONFIG_MQTT_IP);

const char *mqttUsername = CONFIG_MQTT_USER;
const char *mqttPassword = CONFIG_MQTT_PASS;
const char *mqttClientId = CONFIG_MQTT_CLIENTID;
PubSubClient mqttClient(client);

byte mqttWillQoS = 1;
const char *mqttMainTopic = CONFIG_MQTTMAINTOPIC;
const char *mqttStatusTopic = CONFIG_MQTTSTATUSTOPIC;
const char *mqttLogTopic = CONFIG_MQTTLOGTOPIC;
const char *mqttTemperatureTopic = CONFIG_MQTTTEMPERATURETOPIC;
const char *mqttHumidityTopic = CONFIG_MQTTHUMIDITYTOPIC;
const char *mqttWillMessage = CONFIG_MQTTWILLMESSAGE;
const char *mqttSubTopic = CONFIG_MQTTSUBTOPIC;
const char *mqttCommandTopic = CONFIG_MQTTCOMMANDTOPIC;

boolean mqttWillRetain = true;

class RemoteCommandExtended
{
public:
	char *name;
	char *friendly_name;
	int timer;
	uint8_t *bytes;
	uint8_t length;
};

class IthoRemotes
{
public:
	char *remote_id;
	char *friendly_name;
	RemoteCommandExtended remote_commands[CONFIG_MAXCOMMANDSPERREMOTE];
};

class IthoBoxes
{
public:
	char *name;
	char *friendly_name;
	char state[10];
	char oldstate[10];
	int timer;
	int timer_full;
	uint8_t numremotes;
	IthoRemotes remotes[CONFIG_MAXREMOTESPERBOX];
};

class MqttTopics
{
public:
	char State[128];
	char Timer[128];
	char Cmd[128];
	char LastIDindex[128];
};

const uint8_t numberOfIthoBoxes = CONFIG_NUMBEROFITHOBOXES;
MqttTopics mqttTopics[numberOfIthoBoxes];