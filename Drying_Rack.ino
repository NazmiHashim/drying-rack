/*
  ESP32 Smart Drying Rack System using V-One Cloud
*/

#include "VOneMqttClient.h"
#include "DHT.h"
#include <ESP32Servo.h>

int servoSpeed = 20; 
int lastServoPosition = -1;  // Initialize with an invalid position
bool isFanOn = false; // To check whether the dryer fan is activated
bool isDrying = false; // To check whether on dry mode
unsigned long cur = millis();

//define device id
const char* DHT11Sensor = "de2c3a76-d230-40f5-a2b2-adff5e6bd9c9";
const char* RainSensor = "a4c9d411-c5aa-4791-a9e7-dc2c0ff11c10";


//Used Pins
const int btnPin1 = 14;
const int btnPin2 = 33;
const int dht11Pin = 21;
const int rainPin = 35;
const int servoPin = 16;
const int relayPin = 32;

//input sensor
#define DHTTYPE DHT11
DHT dht(dht11Pin, DHTTYPE);

//Create an instance of VOneMqttClient
VOneMqttClient voneClient;

// Create a Servo object
Servo servo;

//last message time
unsigned long lastMsgTime = 0;

void setup_wifi() {
  delay(10);
  // Connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  setup_wifi();
  voneClient.setup();

  pinMode(btnPin1, INPUT_PULLUP);
  pinMode(btnPin2, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, false);

  //sensor
  dht.begin();
  pinMode(rainPin, INPUT);

  // // Initialize the servo
  servo.attach(servoPin, 500, 2500);
  servo.setPeriodHertz(50);
  servo.write(0);  // Start at the neutral position (rack is open)
} 

// Smoothly move the servo to a target position
void moveServoSmooth(int targetAngle, int speed) {
  if (lastServoPosition < targetAngle) {
    for (int pos = lastServoPosition; pos <= targetAngle; pos++) {
      servo.write(pos);     // Incrementally move servo
      delay(speed);         // Control speed with delay
    }
  } else if (lastServoPosition > targetAngle) {
    for (int pos = lastServoPosition; pos >= targetAngle; pos--) {
      servo.write(pos);     // Incrementally move servo
      delay(speed);         // Control speed with delay
    }
  }
  lastServoPosition = targetAngle;  // Update the last position
}

void loop() {
  if (!voneClient.connected()) {
    voneClient.reconnect();
    String errorMsg = "DHTSensor Fail";
    voneClient.publishDeviceStatusEvent(DHT11Sensor, true);
    voneClient.publishDeviceStatusEvent(RainSensor, true);
  }
  voneClient.loop();

  int button1 = !digitalRead(btnPin1); // Read button 1 value
  int button2 = !digitalRead(btnPin2); // Read button 2 value

  cur = millis();
  if (cur - lastMsgTime > INTERVAL) {
    lastMsgTime = cur;

    //Publish humidity and temperature data
    float h = dht.readHumidity();
    int t = dht.readTemperature();

    JSONVar payloadObject;
    payloadObject["Humidity"] = h;
    payloadObject["Temperature"] = t;
    voneClient.publishTelemetryData(DHT11Sensor, payloadObject);

    //Publish rain data
    int raining = !digitalRead(rainPin);
    delay(50);
    voneClient.publishTelemetryData(RainSensor, "Raining", raining);

    // If raining, then it will retract the servo and start the fan
    if (raining == 1 && isDrying == true && isFanOn == false) 
    {
      isFanOn = true;
      digitalWrite(relayPin, true);
      Serial.println("Rain detected! Moving servo to retract position.");
      moveServoSmooth(0, servoSpeed); 
      lastServoPosition = 0;  // Update the last position
      delay(20);
    }
    
    if (button1)  // To move the dryer under the roof 
    {
      isFanOn = false;
      isDrying = false;
      digitalWrite(relayPin, false);
      Serial.println("Stopping. Switching off the power...");
      if (lastServoPosition != 0) {  // Check if the servo is not already at 0
          Serial.println("Rain detected! Moving servo to retract position.");
          moveServoSmooth(0, servoSpeed); 
          lastServoPosition = 0;  // Update the last position
      } else {
          Serial.println("Servo is already at 0 (retracted).");
      }
    } 
    else if (button2) // To activate drying mode
    {
      isDrying = true;
      if (lastServoPosition != 180) {  // Check if the servo is not already at 90
          Serial.println("No rain detected! Moving servo to open position.");
          moveServoSmooth(180, servoSpeed); 
          lastServoPosition = 180;  // Update the last position
      } else {
          Serial.println("Servo is already at fully open.");
      }
    }
  }
}
