#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#define MSG_BUFFER_SIZE 50

const int RED_LED = 10;
const int GREEN_LED = 11;
const int trigPin = 5;
const int echoPin = 18;

/* Supposing to run this test
   in an environment with 20 °C
  ì https://en.wikipedia.org/wiki/Speed_of_sound
   */

const float temperature = 20;
const float vs = 331.45 + 0.62 * temperature;
/* wifi network info */

const char* ssid = "Wifu";
const char* password = "tangente";

/* MQTT server address */
const char *mqtt_server = "broker.mqtt-dashboard.com";

/* MQTT topic */
const char *riverLevelTopic = "river-level";

const char *frequencyTopic = "frequency";

int frequency = 2000;
/* MQTT client management */

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsgTime = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi()
{

  delay(10);

  Serial.println(String("Connecting to ") + ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/* MQTT subscribing callback */

void callback(char* topic, byte* payload, unsigned int length) {
  String mess = "";
  if (String(topic) == String(frequencyTopic)) {
    for (int i = 0; i < length; i++) {
      mess += (char)payload[i];
    }
    frequency = mess.toInt();
  }
}

float getDistance()
{
  /* Triggering stage: sending the impulse */

  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  /* Receiving the echo */

  float tUS = pulseIn(echoPin, HIGH);
  float t = tUS / 1000.0 / 1000.0 / 2;
  float d = t * vs;
  return d;
}

void reconnect()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    setup_wifi();
  }
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = String("esiot-2122-client-") + String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(riverLevelTopic);
      client.subscribe(frequencyTopic);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  setup_wifi();
  randomSeed(micros());
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    reconnect();
  }
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  client.loop();

  unsigned long now = millis();
  if (now - lastMsgTime > frequency) {
    lastMsgTime = now;
    /* creating a msg in the buffer */
    snprintf(msg, MSG_BUFFER_SIZE, "%lf", getDistance());

    //Serial.println(String("Publishing message: ") + msg);
    
    /* publishing the msg */

    client.publish(riverLevelTopic, msg);
  }
}
