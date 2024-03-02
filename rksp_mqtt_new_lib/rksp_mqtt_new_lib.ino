#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>

#include "pwd.h"

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

void connect2wifiAndServer(){
  // attempt to connect to Wifi network:
  if (WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.begin(ssid, password) != WL_CONNECTED) {
      // failed, retry
      Serial.print(".");
      delay(5000);
    }
    
    Serial.println("You're connected to the network");
    Serial.println();
  }

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(mqtt_server);

  if (!mqttClient.connect(mqtt_server, mqtt_port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
    
  connect2wifiAndServer();
  
  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);


  // subscribe to a topic
  mqttClient.subscribe(permission_topic);
  mqttClient.subscribe(value_topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

}

void loop() {
  mqttClient.poll();
}


void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.println("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }
  Serial.println();
  Serial.println();
}
