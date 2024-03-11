#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>
#include "pwd.h"
#include <Adafruit_NeoPixel.h>
#include <Servo.h>

Servo servo;

#define ml_sec 10

#define pump_Pin 16
#define valve_Pin 5

#define switch1 4
#define switch2 0
#define switch3 15

#define strip_pin 14
#define NUMPIXELS 3
Adafruit_NeoPixel pixels(NUMPIXELS, strip_pin, NEO_GRB + NEO_KHZ800);

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

int glass_pos[3] = {0,0,0};

void connect2wifiAndServer() {
  // attempt to connect to Wifi network:
  if (WiFi.status() != WL_CONNECTED) {
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
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
  }
  servo.write(180);
  delay(200);
  servo.write(90);
  delay(200);
  servo.write(0);

}

void setup() {
  pinMode(pump_Pin, OUTPUT);
  pinMode(valve_Pin, OUTPUT);

  pinMode(switch1, INPUT);
  pinMode(switch2, INPUT);
  pinMode(switch3, INPUT);

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pixels.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pixels.clear();
  analogWriteResolution(16);
  servo.attach(2, 544, 2444); //D4
  servo.write(0);
  delay(100);
  servo.write(180);

  connect2wifiAndServer();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
  }

  mqttClient.onMessage(onMqttMessage);
  mqttClient.subscribe(value_topic);
}

void loop() {
  mqttClient.poll();
//  if(digitalRead(switch1))
//    Serial.println("ddd");
  if(!digitalRead(switch1)){
    if(glass_pos[0] == 0){
      glass_pos[0] = 1;
      pixels.setPixelColor(0, pixels.Color(150, 0, 0));
    }
  }else{
    glass_pos[0] = 0;
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  }
  
  if(!digitalRead(switch2)){
    if(glass_pos[1] == 0){
      glass_pos[1] = 1;
      pixels.setPixelColor(1, pixels.Color(150, 0, 0));
    }
  }else{
    glass_pos[1] = 0;
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  }

  if(!digitalRead(switch3)){
    if(glass_pos[2] == 0){
      glass_pos[2] = 1;
      pixels.setPixelColor(2, pixels.Color(150, 0, 0));
    }
  }else{
    glass_pos[2] = 0;
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  }
 
  pixels.show();
}


void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  String top = mqttClient.messageTopic();
  Serial.println("Received a message with topic '");
  Serial.print(top);
  Serial.print(", length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");
  String input = "";
  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    input += (char)mqttClient.read();
  }
  Serial.println(input);
  
  if(top == value_topic){
    Serial.println("START");
    int val = input.toInt();
    if(glass_pos[0] == 1){
      servo.write(45);
      delay(50);
      digitalWrite(pump_Pin, HIGH);
      digitalWrite(valve_Pin, HIGH);
      delay(val*ml_sec);
      digitalWrite(pump_Pin, LOW);
      digitalWrite(valve_Pin, LOW);
      pixels.setPixelColor(0, pixels.Color(0, 150, 0));
      glass_pos[0] = 2;
      pixels.show();
    }
    if(glass_pos[1] == 1){
      servo.write(90);
      delay(50);
      digitalWrite(pump_Pin, HIGH);
      digitalWrite(valve_Pin, HIGH);
      delay(val*ml_sec);
      digitalWrite(pump_Pin, LOW);
      digitalWrite(valve_Pin, LOW);
      pixels.setPixelColor(1, pixels.Color(0, 150, 0));
      glass_pos[1] = 2;
      pixels.show();
    }
    if(glass_pos[2] == 1){
      servo.write(135);
      delay(50);
      digitalWrite(pump_Pin, HIGH);
      digitalWrite(valve_Pin, HIGH);
      delay(val*ml_sec);
      digitalWrite(pump_Pin, LOW);
      digitalWrite(valve_Pin, LOW);
      pixels.setPixelColor(2, pixels.Color(0, 150, 0));
      glass_pos[2] = 2;
      pixels.show();
    }
    servo.write(0);
    
  }
  Serial.println();
}
