#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "insert wifi name here"; // Enter your WiFi name
const char *password = "insert wifi password here";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "192.168.1.32";
const char *topic = "esp32";

WiFiClient espClient;
PubSubClient client(espClient);

// LED definition
#define PIN_LED 2

void setup() {
  // Initialize the BUILTIN_LED pin as an output
  pinMode(PIN_LED, OUTPUT);
  
   // Software serial is set to 115200
   Serial.begin(115200);
   setup_wifi();
   //connecting to a mqtt broker
   client.setServer(mqtt_broker, 1883);
   client.setCallback(callback);

   // Connecting to MQTT
   while (!client.connected()) {
       String clientID = "esp32-clientID";
       Serial.println("Now attempting to connect to the MQTT server");
       if (client.connect(clientID.c_str())) 
       {
           Serial.println("MQTT server connected");
       } 
       else 
       {
           Serial.print("failed with state ");
           Serial.print(client.state());
           Serial.println();
           delay(2000);
       }
 }
 // publish and subscribe
 client.publish(topic, "Hello, you have connected to ESP32!");
 client.subscribe(topic);
}



void setup_wifi() {

  delay(10);
  // Start off by connecting to WiFi netword
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  Serial.println("");
  Serial.println("WiFi connected");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') 
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on
   else 
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off

}

void loop() {
 client.loop();
}